#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "../src/Reflection.hpp"

class Component : public ReflectionObject
{
public:
	Component() {};
	~Component() {};

	void setName(std::string const& name) { m_name = name; }
	std::string getName() { return m_name; }

	void setCount(int count) { m_count = count; }
	int getCount() { return m_count; }

	void serialize(ReflectionSystem& system, std::string const& identifier) const
	{
        system.create(identifier);
        system.save("name", m_name);
        system.save("count", m_count);
        system.end();
	}

	bool deserialize(ReflectionSystem& system, std::string const& identifier)
	{
	    if (system.read(identifier))
        {
            system.load("name", m_name);
            system.load("count", m_count);
            system.end();
            return true;
        }
        return false;
	}

private:
	std::string m_name;
	int m_count;
};

#endif // COMPONENT_HPP
