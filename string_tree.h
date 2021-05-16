#pragma once
#include <string>
#include <memory>
#include <map>

namespace smnet{


	class StringTreeValue;
	typedef std::map<std::string, StringTreeValue> StringTreeMap;

	// a stringMap and it can
	class StringTreeValue{
		public:
			virtual std::string get() = 0;
			virtual void put(const std::string& value) = 0;
			virtual std::shared_ptr<StringTreeValue> get(const std::string& key) = 0;
			virtual void put(const std::string& key, const std::shared_ptr<StringTreeValue> value) = 0;
			virtual std::shared_ptr<StringTreeValue> clone();
		public:
			virtual ~StringTreeValue(){}
		public:
			static std::shared_ptr<StringTreeValue> create(const std::string& value = "");
	};


}

