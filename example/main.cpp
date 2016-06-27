#include <iostream>

#include "../src/XMLSystem.hpp"
#include "../src/JSONSystem.hpp"

#include "Actor.hpp"
#include "Component.hpp"

void serialize(ReflectionSystem& system)
{
    Component c1;
	c1.setName("Used1");
	c1.setCount(123);
	Component c2;
	c2.setName("Used2");
	c2.setCount(456);
	Component c3;
	c3.setName("Used3");
	c3.setCount(789);
	Actor a;
    a.setName("Jack");
    a.getMain().setName("Camera");
    a.getMain().setCount(48);
    a.addValue(1.1f);
    a.addValue(3.14f);
    a.addValue(9.8f);
    a.addComponent("Box",24);
    a.addComponent("Audio",12);
    a.addComponent("Shader",6);
    a.setProperty("debug", 12.34f);
    a.setProperty("sizex", 800.f);
    a.setProperty("sizey", 600.f);
    a.storeComponent(c1.getName(),c1);
    a.storeComponent(c2.getName(),c2);
    a.storeComponent(c3.getName(),c3);

    a.serialize(system, "Actor");
}

void serialize(bool json, bool xml)
{
    if (json)
    {
        std::cout << "====SERIALIZE JSON====" << std::endl;
        JSONSystem json;
        json.openDocument("example/test.json",true);
        serialize(json);
        json.saveDocument();
        std::cout << "OK" << std::endl;
    }
    if (xml)
    {
        std::cout << "====SERIALIZE XML====" << std::endl;
        XMLSystem xml;
        xml.openDocument("example/test.xml",true);
        serialize(xml);
        xml.saveDocument();
        std::cout << "OK" << std::endl;
    }
}

void deserialize(ReflectionSystem& system)
{
    Actor a;
    if (a.deserialize(system,"Actor"))
    {
        int sum = 0;
        if (a.getName() == "Jack") sum++;
        if (a.getMain().getName() == "Camera") sum++;
        if (a.value(1) == 3.14f) sum++;
        if (a.getComponent(1).getName() == "Audio") sum++;
        if (a.getProperty("sizey") == 600) sum++;
        if (a.getUsedComponent("Used1").getCount() == 123) sum++;
        std::cout << ((sum == 6) ? std::string("OK") : std::string("Error")) << std::endl;
    }
    else
    {
        std::cout << "ERROR" << std::endl;
    }
}

void deserialize(bool json, bool xml)
{
    if (json)
    {
        std::cout << "====DESERIALIZE JSON====" << std::endl;
        JSONSystem json;
        json.openDocument("example/test.json");
        deserialize(json);
    }
    if (xml)
    {
        std::cout << "====DESERIALIZE XML====" << std::endl;
        XMLSystem xml;
        xml.openDocument("example/test.xml");
        deserialize(xml);
    }
}

int main()
{
    serialize(true, true);
    deserialize(true, true);
    return 0;
}
