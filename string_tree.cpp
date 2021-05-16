#include "string_tree.h"
#include "assert.h"
using namespace std;
namespace smnet{
	enum StringTreeValueType{
		PURE_STRING,
		STRING_MAP,
	};
	typedef std::shared_ptr<StringTreeValue> StringTreeValuePtr;
	class PureStringValue;
	class MapValue;

	class StringTreeValueImpl: public StringTreeValue{
		public:
		   	StringTreeValueImpl(const std::string& value = "");
		   	StringTreeValueImpl(const map<string, std::shared_ptr<StringTreeValue> >& sMap = {});
		public:
			std::string get() override{
				return impl->get();
			}

			void put(const std::string &value) override{
				impl->put(value);
			}

			StringTreeValuePtr get(const std::string &key) override{
				return impl->get(key);
			}
			
			void put(const std::string &key, const std::shared_ptr<StringTreeValue> value) override;

			std::shared_ptr<StringTreeValue> clone() override{
				return impl->clone();
			}

		private:
			std::shared_ptr<StringTreeValue> impl;
			StringTreeValueType type;
	};
	static const std::string USE_PURE_STRING_AS_MAP(
			"PureString Do Not Has put(Key, value) and get(key) method\n"
			"now you should tell github.com/ProtossGenius he make a big bug about ...\n");
	class PureStringValue: public StringTreeValue{
		public:
			PureStringValue(const std::string& value){
				m_value = value;
			}
		public:
			std::string get() override{
				return m_value;
			}

			void put(const std::string &value) override{
				m_value = value;
			}

			std::shared_ptr<StringTreeValue> clone() override{
				return std::make_shared<PureStringValue>(*this);
			}

			std::shared_ptr<StringTreeValue> get(const std::string &key) override{
				doThrow<std::runtime_error>(USE_PURE_STRING_AS_MAP);
				return nullptr;
			}

			void put(const std::string& key, const std::shared_ptr<StringTreeValue> value) override{
				doThrow<std::runtime_error>(USE_PURE_STRING_AS_MAP);
			}

		private:
			std::string m_value;
	};

	class MapValue: public StringTreeValue{
		public:
			MapValue(const std::string& value):m_value(value), m_map(){
				
			}
			MapValue(const MapValue& rhs):m_value(rhs.m_value) {
				for(auto it: rhs.m_map){
					m_map[it.first] = it.second->clone();
				}
			}
		public:
			std::string get() override{
				return m_value;
			}

			void put(const std::string &value) override{
				m_value = value;
			}

			std::shared_ptr<StringTreeValue> clone() override{
				return std::make_shared<MapValue>(*this);
			}
			
			std::shared_ptr<StringTreeValue> get(const std::string &key) override{
				return m_map[key];
			}

			void put(const std::string& key, const std::shared_ptr<StringTreeValue> value) override{
				m_map[key] = value->clone();
			}
			
		private:
			std::string m_value;
			std::map<std::string, std::shared_ptr<StringTreeValue>> m_map;

	};

	void StringTreeValueImpl::put(const std::string& key, StringTreeValuePtr value){
		if(this->type == StringTreeValueType::PURE_STRING){
			impl = std::make_shared<MapValue>(impl->get());
		}
		impl->put(key, value->clone());
	}

	StringTreeValueImpl::StringTreeValueImpl(const std::string& value):
	impl(std::make_shared<PureStringValue>(value)), type(StringTreeValueType::PURE_STRING){}

	std::shared_ptr<StringTreeValue> StringTreeValue::create(const std::string& value){
		return std::make_shared<StringTreeValueImpl>(value);
	}
}
