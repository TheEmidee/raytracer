#pragma once

#include <string>
#include <memory>

#include "json.hpp"
using nlohmann::json;

class RayTracer;
class Hitable;
class Material;

class FileLoader
{
public:

    FileLoader( std::unique_ptr<RayTracer> & ray_tracer, const std::string & file_name );

private:

    bool CreateHitable( std::shared_ptr< Hitable > & hitable, const std::string & key, const json & hitable_json ) const;
    void GenerateWorld( std::vector< std::shared_ptr< Hitable > > & hitables, const json & world_json ) const;
};