#ifndef XMLSYSTEM_HPP
#define XMLSYSTEM_HPP

#include <cstring> // TEMP

#include <fstream>
#include "../extlibs/pugixml.hpp"
#include "Reflection.hpp"

class XMLSystem : public ReflectionSystem
{
    public:
        XMLSystem() {};
        ~XMLSystem() {};

        bool openDocument(std::string const& filename, bool erase = false)
        {
            bool ret = true;
            m_filename = filename;
            if (erase)
            {
                m_doc.append_child("Main");
                saveDocument();
            }
            else
            {
                std::ifstream file(filename);
                if (file)
                {
                    m_doc.load(file);
                    if (!m_doc.child("Main"))
                    {
                        m_doc.append_child("Main");
                    }
                }
                else
                {
					m_doc.append_child("Main");
                    ret = false;
                }
                file.close();
            }

            m_actualNode = m_doc.child("Main");
            return ret;
        }

        void saveDocument(bool readable = true)
        {
            if (readable)
            {
                m_doc.save_file(m_filename.c_str());
            }
            else
            {
                m_doc.save_file(m_filename.c_str(),"");
            }
        }

        void create(std::string const& identifier)
        {
            if (m_actualNode.child(identifier.c_str()))
            {
                m_actualNode.remove_child(identifier.c_str());
            }
            m_actualNode = m_actualNode.append_child(identifier.c_str());
        }

        void save(std::string const& identifier, std::string const& value)
        {
            if (m_actualNode.attribute(identifier.c_str()))
            {
               m_actualNode.attribute(identifier.c_str()) = value.c_str();
            }
            else
            {
                m_actualNode.append_attribute(identifier.c_str()) = value.c_str();
            }
        }

        void end()
        {
            m_actualNode = m_actualNode.parent();
        }

        bool read(std::string const& identifier)
        {
            if (m_actualNode.child(identifier.c_str()))
            {
                m_actualNode = m_actualNode.child(identifier.c_str());
                return true;
            }
            return false;
        }

        bool load(std::string const& identifier, std::string& value)
        {
            if (m_actualNode.attribute(identifier.c_str()))
            {
                value = m_actualNode.attribute(identifier.c_str()).value();
                return true;
            }
            return false;
        }

    private:
        std::string m_filename;
        pugi::xml_document m_doc;
        pugi::xml_node m_actualNode;
};

#endif // XMLSYSTEM_HPP
