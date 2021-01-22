#ifndef CHANNEL_H_IZJI8D3M
#define CHANNEL_H_IZJI8D3M

#include <queue>
#include <memory>
#include <mutex>
#include "lockm.h"

namespace smnet{
	template<typename Type>
	class channel {
		public:
			channel(size_t maxSize = 0):_MAX_SIZE(maxSize), _exp(nullptr), _alive(true){
				if(_MAX_SIZE == 0){
					canNotWriteIn();
				}else{
					queueEmpty();
				}
			}

		public:
			void push(const Type& val){
				bool noSpace = false;
				{
					SMLockMgr _(_tsafe);
					if (_MAX_SIZE == 0 || _quu.size() == _MAX_SIZE - 1){
						noSpace = true;
					}
					if(_MAX_SIZE != 0 && _quu.empty()){
						queueNotEmpty(); //because will push one value.
					}
					_quu.push(val);

				}

				if(noSpace){
					canNotWriteIn();
				}
			}

			void emplace(Type&& val){
				bool noSpace = false;
				{
					SMLockMgr _(_tsafe);
					if (_MAX_SIZE == 0 || _quu.size() == _MAX_SIZE - 1){
						noSpace = true;
					}
					if(_MAX_SIZE != 0 && _quu.empty()){
						queueNotEmpty(); //because will push one value.
					}
					_quu.emplace(val);

				}

				if(noSpace){
					canNotWriteIn();
				}

			}

			bool empty(){
				SMLockMgr _(_tsafe);
				return _quu.empty();
			}

			size_t size(){
				SMLockMgr _(_tsafe);
				return _quu.size();
			}

			void setExport(Type* out){
				_exp = out;
			}

			//one_thread_get should only call from one thread. it's not thread-safe. or willcaused dead lock.
			void one_thread_get(){
				bool isQueueEmpty = false;
				{//check if need Lock;
					SMLockMgr _(_tsafe);
					if(_MAX_SIZE == 0 || _quu.size() == _MAX_SIZE-1){
						canWriteIn();
					}
					if (_MAX_SIZE != 0 && _quu.empty()){
						isQueueEmpty = true;
					}
				}

				if(isQueueEmpty){
					queueEmpty();
				}

				{
					SMLockMgr _(_tsafe);
					(*_exp) = std::move(_quu.front());
					_quu.pop();
				}
			}

			void close(){
				SMLockMgr _(_tsafe);
				_alive = false;
			}

			bool alive(){
				SMLockMgr _(_tsafe);
				return _alive;
			}
		private:
			void queueEmpty(){
				_readLock.lock();
			}

			void queueNotEmpty(){
				_readLock.unlock();
			}
			
			void canNotWriteIn(){
				_writeLock.lock();
			}
			void canWriteIn(){
				_writeLock.unlock();
			}
		private:
			const size_t _MAX_SIZE;
			std::queue<Type> _quu;
			std::mutex _tsafe;//for thread safe.
			std::mutex _readLock;    //read means read from channel;
			std::mutex _writeLock;   //write means write to channel;
			Type* _exp;
			bool _alive;
	};

	template<typename Type>
	std::shared_ptr<channel<Type> > make_chan(size_t maxSize = 0){
		return std::make_shared<channel<Type> >(maxSize);
	}
}

#endif /* end of include guard: CHANNEL_H_IZJI8D3M */
