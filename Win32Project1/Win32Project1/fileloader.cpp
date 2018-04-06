#include "fileloader.h"

#include <fstream>
#include <experimental/filesystem>

#include "raytracer.h"
#include "world.h"
#include "hitable.h"
#include "camera.h"
#include "material.h"
#include "sphere.h"
#include "resource_manager.h"

namespace fs = std::experimental::filesystem;

FileLoader::FileLoader( std::unique_ptr<RayTracer> & ray_tracer, const std::string & file_name )
{
    if ( file_name.empty() )
    {
        throw new std::exception();
    }

    fs::path json_folder = fs::current_path().append( "JSON" );

    if ( !fs::exists( json_folder ) )
    {
        throw new std::exception();
    }

    fs::path path = json_folder / file_name;
    path.replace_extension( ".json" );

    if ( !fs::exists( path ) )
    {
        throw new std::exception();
    }

    std::ifstream file_stream( path.string() );
    nlohmann::json json;
    file_stream >> json;

    const auto raytracer_json = json[ "raytracer" ];

    CameraParameters camera_parameters = raytracer_json[ "camera" ];
    auto camera = std::make_unique< Camera >( camera_parameters );

    TextureResourceManager::Instance().Load( raytracer_json[ "textures" ] );
    MaterialResourceManager::Instance().Load( raytracer_json[ "materials" ] );

    std::vector< std::shared_ptr< Hitable > > hitables;
    GenerateWorld( hitables, raytracer_json[ "world" ] );

    uint32_t state = rand() * 9781 | 1;

    auto world = std::make_unique< World >( hitables );

    RayTracerParameters ray_tracer_parameters = raytracer_json[ "parameters" ];

    ray_tracer = std::make_unique< RayTracer >( ray_tracer_parameters, std::move( camera ), std::move( world ) );
}

void FileLoader::GenerateWorld( std::vector< std::shared_ptr< Hitable > > & hitables, const json & world_json ) const
{
    for ( const auto & hitable_json : world_json )
    {
        for ( json::const_iterator ite = hitable_json.begin(); ite != hitable_json.end(); ++ite )
        {
            std::shared_ptr< Hitable > hitable;

            if ( CreateHitable( hitable, ite.key(), ite.value() ) )
            {
                hitables.push_back( hitable );
            }
        }
    }
}

bool FileLoader::CreateHitable( std::shared_ptr< Hitable > & hitable, const std::string & key, const json & hitable_json ) const
{
    if ( key == "sphere" )
    {
        Vec3 center = hitable_json[ "center" ];
        float radius = hitable_json[ "radius" ];

        const auto & material_json = hitable_json[ "material" ];

        std::shared_ptr< Material > material;

        if ( material_json.is_object() )
        {
            if ( !MaterialResourceManager::Instance().CreateResourceFromJson( material, material_json ) )
            {
                return false;
            }
        }
        else if ( !MaterialResourceManager::Instance().Get( material, material_json.get< std::string >() ) )
        {
            return false;
        }

        hitable = std::make_shared< Sphere >( center, radius, material );

        return true;
    }

    return false;
}