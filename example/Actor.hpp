#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "../src/Reflection.hpp"
#include "Component.hpp"

class Actor : public ReflectionObject
{
public:
	Actor() {};
	~Actor() {};

	void setName(std::string const& name) { m_name = name; }
	std::string getName() { return m_name; }

	Component& getMain() { return m_main; }

	void addValue(float value) { m_values.push_back(value); }
	float value(std::size_t index) { return m_values.at(index); }

	void addComponent(std::string const& name, int count) { Component c; c.setName(name); c.setCount(count); m_components.push_back(c); }
	Component& getComponent(std::size_t index) { return m_components.at(index); }

	void setProperty(std::string const& name, float value) { m_properties[name] = value; }
	float getProperty(std::string const& name) { return m_properties[name]; }

	void storeComponent(std::string const& name, Component const& component) { m_usedComponents[name] = component; }
	Component& getUsedComponent(std::string const& name) { return m_usedComponents[name]; }

	void serialize(ReflectionSystem& system, std::string const& identifier) const
	{
	    system.create(identifier);
		system.save("name", m_name);
		system.save("Values", m_values);
		system.save("Properties", m_properties);
		system.saveComplex("Main", m_main);
		system.saveComplex("Components", m_components);
		system.saveComplex("Used", m_usedComponents);
        system.end();
	}

	bool deserialize(ReflectionSystem& system, std::string const& identifier)
	{
	    if (system.read(identifier))
        {
            system.load("name", m_name);
            system.load("Values", m_values);
            system.load("Properties", m_properties);
            system.loadComplex("Main", m_main);
            system.loadComplex("Components", m_components);
            system.loadComplex("Used", m_usedComponents);
            system.end();
            return true;
        }
        return false;
	}

private:
    std::string m_name;
    Component m_main;
    std::vector<float> m_values;
    std::vector<Component> m_components;
    std::map<std::string,float> m_properties;
    std::map<std::string,Component> m_usedComponents;
};

#endif // ACTOR_HPP
