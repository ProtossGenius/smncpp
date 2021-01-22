#include "ticker.h"
#include "lockm.h"
#include <thread>
using namespace std;
namespace smnet{
	typedef SMLockMgr lockm;
	void Ticker::close(){
		_droped = true;
	}
	
	void Ticker::setTickDo(std::function<void()> tickDo){
		lockm _(this->_tsafe);
		this->_tickDo = tickDo;
	}

	bool Ticker::tick(){
		lockm _(this->_tsafe);
		_chan.one_thread_get();
		return !_droped;
	}

	void Ticker::put(){
		this->_tickDo();
		if (!_chan.empty()){
			return ;
		}

		_chan.push(0);
	}
	
	int TickUnit::tick(int ms){
		_cur += ms;
		if(_cur < _waitTime){
			return _waitTime - _cur;
		}
		_cur = 0;
		for (auto it = _ticks.begin(); it != _ticks.end(); ){
			auto& cur = *it;
			if(cur->_droped || cur.use_count() == 1){
				_ticks.erase(it++);
				continue;
			}

			cur->put();
			++it;
		}

		return _waitTime;
	}
	

	std::shared_ptr<Ticker> TickUnit::getTicker(){
		std::shared_ptr<Ticker> res(new Ticker());
		this->_ticks.push_back(res);
		return res;
	}

	TickManager::TickManager(){
		_emptyLock.lock();
		_thread = std::shared_ptr<std::thread>(new std::thread(&TickManager::tickLoop, this))	;
	}

	TickManager::~TickManager(){
		_thread->detach();
	}

	TickManager& TickManager::Instance(){
		static TickManager ins;
		return ins;
	}

	void TickManager::tickLoop(){
		int wTime = 1;
		while(true){
			bool isEmpty = false;
			{
				lockm _(this->_tsafe);
				if (this->_tunits.empty()){
					isEmpty = true;
				}
			}

			if (isEmpty){
				this->_emptyLock.lock();
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(wTime));
			int minWait = -1;
			{
				lockm _(_tsafe);
				for(auto it = this->_tunits.begin(); it != this->_tunits.end();){
					auto& cur = it->second;
					//delete no-use TickUnit;
					if (cur->empty()){
						_tunits.erase(it++);
						continue;
					}
					//do tick
					if (minWait == -1){
						minWait = cur->tick(wTime);
					}else{
						minWait = std::min(minWait, cur->tick(wTime));
					}
				
					++it;
				}
			}
			wTime = 1;
			if (minWait != -1){
				wTime = minWait;
			}
		}
	}


	std::shared_ptr<Ticker> TickManager::getTicker(int tickMs){
		lockm _(_tsafe);
		bool isEmpty = this->_tunits.empty();
		auto res = _get_ticker(tickMs);
		if(isEmpty && res != nullptr){
			_emptyLock.unlock();
			return res;
		} 

		return res;
	}

	std::shared_ptr<Ticker> TickManager::_get_ticker(int tickMs){
		if (tickMs == 0){
			return nullptr;
		}

		auto ptr = this->_tunits.find(tickMs);
		if (ptr == this->_tunits.end()){
			auto tu =  std::make_shared<TickUnit>(tickMs);
			_tunits[tickMs] = tu;
			return tu->getTicker();
		}

		return ptr->second->getTicker();
	}
}
