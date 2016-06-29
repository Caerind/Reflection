#ifndef REFLECTIONSYSTEM_HPP
#define REFLECTIONSYSTEM_HPP

#include <map>
#include <sstream>
#include <vector>
#include <string>

class ReflectionSystem;

class ReflectionObject
{
public:
	ReflectionObject() {};
	~ReflectionObject() {};

	virtual void serialize(ReflectionSystem& system, std::string const& identifier) const {};
	virtual bool deserialize(ReflectionSystem& system, std::string const& identifier) { return false; };
};

class ReflectionSystem
{
public:
	ReflectionSystem() {};
	~ReflectionSystem() {};

	// Write
	virtual void create(std::string const& identifier) {};
	virtual void save(std::string const& identifier, std::string const& value) {};
	virtual void save(std::string const& identifier, int const& value) { save(identifier, toString(value)); };
	virtual void save(std::string const& identifier, float const& value) { save(identifier, toString(value)); };
	virtual void save(std::string const& identifier, char const& value) { save(identifier, toString(value)); };
	virtual void save(std::string const& identifier, double const& value) { save(identifier, toString(value)); };
	virtual void save(std::string const& identifier, unsigned int const& value) { save(identifier, toString(value)); };
	virtual void save(std::string const& identifier, bool const& value) { save(identifier, toString(value)); };
	template <typename T>
	void save(std::string const& identifier, std::vector<T> const& vector)
	{
	    create(identifier);
        save("size", vector.size());
        for (std::size_t i = 0; i < vector.size(); ++i)
        {
            save("i" + toString(i), vector.at(i));
        }
        end();
	}
	template <typename T>
	void saveComplex(std::string const& identifier, std::vector<T> const& vector)
	{
        create(identifier);
        save("size", vector.size());
        for (std::size_t i = 0; i < vector.size(); ++i)
        {
            vector.at(i).serialize(*this, "i" + toString(i));
        }
        end();
	}
	template <typename K, typename T>
	void save(std::string const& identifier, std::map<K,T> const& map)
	{
	    create(identifier);
        save("size", map.size());
        std::size_t i = 0;
        for (auto itr = map.begin(); itr != map.end(); itr++)
        {
            save("i" + toString(i), std::string(toString(itr->first) + "||" + toString(itr->second)));
            i++;
        }
        end();
	}
	template <typename K, typename T>
	void saveComplex(std::string const& identifier, std::map<K,T> const& map)
	{
        create(identifier);
        save("size", map.size());
        std::size_t i = 0;
        for (auto itr = map.begin(); itr != map.end(); itr++)
        {
            std::string key = toString(itr->first);
            save("i" + toString(i), key);
            itr->second.serialize(*this, key);
            i++;
        }
        end();
	}
	template <typename T>
	void saveComplex(std::string const& identifier, T const& t)
	{
	    t.serialize(*this,identifier);
	}
	virtual void end() {};

    // Read
    virtual bool read(std::string const& identifier) { return false; };
	virtual bool load(std::string const& identifier, std::string& value) { return false; };
	virtual bool load(std::string const& identifier, int& value) { std::string temp = ""; if (load(identifier, temp)) { value = fromString<int>(temp); return true; } return false; };
	virtual bool load(std::string const& identifier, float& value) { std::string temp = ""; if (load(identifier, temp)) { value = fromString<float>(temp); return true; } return false; };
	virtual bool load(std::string const& identifier, char& value) { std::string temp = ""; if (load(identifier, temp)) { value = fromString<char>(temp); return true; } return false; };
	virtual bool load(std::string const& identifier, double& value) { std::string temp = ""; if (load(identifier, temp)) { value = fromString<double>(temp); return true; } return false; };
	virtual bool load(std::string const& identifier, unsigned int& value) { std::string temp = ""; if (load(identifier, temp)) { value = fromString<unsigned int>(temp); return true; } return false; };
	virtual bool load(std::string const& identifier, bool& value) { std::string temp = ""; if (load(identifier, temp)) { value = fromString<bool>(temp); return true; } return false; };
	virtual std::string load(std::string const& identifier) { std::string temp = ""; load(identifier, temp); return temp; };
	template <typename T>
	bool load(std::string const& identifier, std::vector<T>& vector)
	{
	    bool ret = true;
        if (read(identifier))
        {
            std::size_t size;
            if (load("size", size))
            {
                vector.clear();
                for (std::size_t i = 0; i < size; ++i)
                {
                    T value;
                    if (!load("i" + toString(i),value))
                    {
                        value = T();
                        ret = false;
                    }
                    vector.push_back(value);
                }
                end();
                return ret;
            }
            end();
        }
        return false;
	}
	template <typename T>
	bool loadComplex(std::string const& identifier, std::vector<T>& vector)
	{
	    bool ret = true;
        if (read(identifier))
        {
            std::size_t size;
            if (load("size", size))
            {
                vector.clear();
                for (std::size_t i = 0; i < size; ++i)
                {
                    vector.push_back(T());
                    if (!loadComplex("i" + toString(i), vector.back()))
                    {
                        ret = false;
                    }
                }
                end();
                return ret;
            }
            end();
        }
        return false;
	}
	template <typename K, typename T>
	bool load(std::string const& identifier, std::map<K,T>& map)
	{
	    bool ret = true;
        if (read(identifier))
        {
            std::size_t size;
            if (load("size", size))
            {
                map.clear();
                for (std::size_t i = 0; i < size; ++i)
                {
                    std::string temp;
                    if (load("i" + toString(i), temp))
                    {
                        std::size_t f = temp.find_first_of("||");
                        K k;
                        T t;
                        if (f != std::string::npos)
                        {
                            k = fromString<K>(temp.substr(0,f));
                            t = fromString<T>(temp.substr(f+2));
                        }
                        else
                        {
                            ret = false;
                        }
                        map.emplace(k, t);
                    }
                    else
                    {
                        end();
                        return false;
                    }
                }
                end();
                return ret;
            }
            end();
        }
        return false;
	}
	template <typename K, typename T>
	bool loadComplex(std::string const& identifier, std::map<K,T>& map)
	{
	    bool ret = true;
        if (read(identifier));
        {
            std::size_t size;
            if (load("size", size))
            {
                map.clear();
                for (std::size_t i = 0; i < size; ++i)
                {
                    std::string key;
                    if (load("i" + toString(i), key))
                    {
                        T t;
                        if (!loadComplex(key, t))
                        {
                            ret = false;
                            t = T();
                        }
                        map.emplace(fromString<K>(key), t);
                    }
                    else
                    {
                        end();
                        return false;
                    }
                }
                end();
                return ret;
            }
            end();
        }
        return false;
	}
	template <typename T>
	bool loadComplex(std::string const& identifier, T& t)
	{
        return t.deserialize(*this, identifier);
	}

protected:
    template <typename T>
    std::string toString(const T& value)
    {
        std::ostringstream os;
        os << value;
        return os.str();
    }

    template <typename T>
    T fromString(const std::string& str)
    {
        std::istringstream i(str);
        T t;
        i >> t;
        return t;
    }
};

#endif // REFLECTIONSYSTEM_HPP
