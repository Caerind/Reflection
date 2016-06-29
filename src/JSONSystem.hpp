#ifndef JSONSYSTEM_HPP
#define JSONSYSTEM_HPP

#include <fstream>
#include <vector>
#include "../extlibs/json.hpp"
#include "Reflection.hpp"

class JSONSystem : public ReflectionSystem
{
    public:
        JSONSystem() { m_actualNode = nullptr; };
        ~JSONSystem() {};

        bool openDocument(std::string const& filename, bool erase = false)
        {
            bool ret = true;
            m_filename = filename;
            if (erase)
            {
                m_json["Main"];
                saveDocument();
            }
            else
            {
                std::ifstream file(m_filename);
                if (file)
                {
                    file >> m_json;
                    m_json["Main"];
                }
                else
                {
                    ret = false;
                }
            }
            m_actualNode = &m_json;
            create("Main");
            return ret;
        }

        void saveDocument(bool readable = true)
        {
            std::ofstream file(m_filename);
            if (file)
            {
                if (readable)
                {
                    file << std::setw(4) << m_json;
                }
                else
                {
                    file << m_json;
                }
            }
        }

        void create(std::string const& identifier)
        {
            if (m_actualNode != nullptr)
            {
                m_actualNode = &((*m_actualNode)[identifier]);
                m_step.push_back(identifier);
            }
        }

        void save(std::string const& identifier, std::string const& value)
        {
            if (m_actualNode != nullptr)
            {
                (*m_actualNode)[identifier] = value;
            }
        }

        void end()
        {
            if (m_actualNode != nullptr)
            {
                m_step.pop_back();

                m_actualNode = &m_json;

                std::vector<std::string> c = m_step;
                m_step.clear();
                for (std::size_t i = 0; i < c.size(); i++)
                {
                    read(c[i]);
                }
            }
        }

        bool read(std::string const& identifier)
        {
            if (m_actualNode != nullptr)
            {
                m_actualNode = &((*m_actualNode)[identifier]);
                m_step.push_back(identifier);
                return true;
            }
            return false;
        }

        bool load(std::string const& identifier, std::string& value)
        {
            if (m_actualNode != nullptr)
            {
                value = (*m_actualNode)[identifier];
                return true;
            }
            return false;
        }

    private:
        std::string m_filename;
        nlohmann::json m_json;
        nlohmann::json* m_actualNode;
        std::vector<std::string> m_step;
};

#endif // JSONSYSTEM_HPP
