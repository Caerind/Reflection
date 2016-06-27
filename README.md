# Reflection
============

The goal of this library is to be easy to use and support XML and JSON with the same interface.

Features :
----------
- XML and JSON systems
- Simple interface
- Serialize complex objects
- Serialize std::vector<T> and std::map<K,T>

Usage :
-------

```c++
class A : public ReflectionObject
{
	public:
		A()
		{
			// Init
		}
		
		void serialize(ReflectionSystem& system, std::string const& identifier)
		{
			system.create(identifier);
			system.save("name", m_name);
			system.save("values", m_values);
			system.saveComplex("test", m_test);
			system.saveComplex("others", m_others);
			system.end();
		}
		
		bool deserialize(ReflectionSystem& system, std::string const& identifier)
		{
			if (system.read(identifier))
			{
				system.load("name", m_name); // You can add safety check for each functions
				system.load("values", m_values);
				system.loadComplex("test", m_test);
				system.loadComplex("others", m_others);
				return true;
			}
			return false;
		}
	
	private:
		std::string m_name;
		std::vector<int> m_values;
		B m_test; // B also inherit from ReflectionObject
		std::vector<B> m_others;
}

int main()
{
	JSONSystem json;
	json.openDocument("test.json"); // openDocument(filename, bool erase = false) -> true = new file
	
	A object;
	a.serialize(json, "A");
	
	json.saveDocument(); // saveDocument(bool readable = true) -> setting it to false can be useful for networking
	
	A test;
	test.deserialize(json, "A");
	
	// test == a
	
	return 0;
}
```

Problems :
----------
The support of both file format add few restrictions about the serialization. I had to find a way to make it "cross-file"
Because of XML : Array and Map can't be parsed with the JSON standard.
Because of JSON : You can't have few objects at the same node level with the same name.

Dependencies :
----------
XML : https://github.com/zeux/pugixml
JSON : https://github.com/nlohmann/json