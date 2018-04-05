#pragma once

#include <map>
#include <utility>
#include <string>

#include "json.hpp"
using nlohmann::json;

#include "helpers.h"

#include "material.h"

template< class _T_ >
class ResourceManager
{
public:

    virtual void Load( const json & json_object ) = 0;

    bool Get( std::shared_ptr< _T_ > & resource, const std::string & key ) const
    {
        auto ite = resourceMap.find( key );

        if ( ite != resourceMap.end() )
        {
            resource = ite->second;
            return true;
        }

        return false;
    }

protected:

    std::map< std::string, std::shared_ptr< _T_ > > resourceMap;
};

class MaterialResourceManager : public ResourceManager< Material >
{
public:

    static auto & Instance()
    {
        static MaterialResourceManager instance;
        return instance;
    }

    virtual void Load( const json & json_object ) override
    {
        for ( const auto & named_aterial_json : json_object )
        {
            for ( json::const_iterator ite = named_aterial_json.begin(); ite != named_aterial_json.end(); ++ite )
            {
                std::shared_ptr< Material > material_ptr;

                if ( CreateMaterialFromJson( material_ptr, ite.value() ) )
                {
                    resourceMap.insert( std::make_pair( ite.key(), material_ptr ) );
                }
            }
        }
    }

    bool CreateMaterialFromJson( std::shared_ptr< Material > & material_ptr, const json & material_json ) const
    {
        for ( json::const_iterator ite = material_json.begin(); ite != material_json.end(); ++ite )
        {
            const auto material_type = ite.key();

            if ( material_type == "lambert" )
            {
                Vec3 albedo = ite.value()[ "albedo" ];
                material_ptr = std::make_shared< MaterialLambert >( albedo );

                return true;
            }
            if ( material_type == "metal" )
            {
                Vec3 albedo = ite.value()[ "albedo" ];
                float fuzzyness = ite.value()[ "fuzzyness" ];
                material_ptr = std::make_shared< MaterialMetal >( albedo, fuzzyness );

                return true;
            }
            if ( material_type == "dielectric" )
            {
                float refraction_index = ite.value()[ "refractionIndex" ];
                material_ptr = std::make_shared< MaterialDiElectric >( refraction_index );

                return true;
            }
        }

        return false;
    }
};