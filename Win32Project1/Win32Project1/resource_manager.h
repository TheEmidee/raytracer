#pragma once

#include <map>
#include <utility>
#include <string>

#include "json.hpp"
using nlohmann::json;

#include "helpers.h"

#include "material.h"
#include "texture.h"

template< class _T_ >
class ResourceManager
{
public:

    void Load( const json & json_object )
    {
        for ( const auto & named_resource_json : json_object )
        {
            for ( json::const_iterator ite = named_resource_json.begin(); ite != named_resource_json.end(); ++ite )
            {
                std::shared_ptr< _T_ > resource_ptr;

                if ( CreateResourceFromJson( resource_ptr, ite.value() ) )
                {
                    resourceMap.insert( std::make_pair( ite.key(), resource_ptr ) );
                }
            }
        }
    }

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

    virtual bool CreateResourceFromJson( std::shared_ptr< _T_ > & resource_ptr, const json & resource_json ) const = 0;

protected:

    std::map< std::string, std::shared_ptr< _T_ > > resourceMap;
};

class TextureResourceManager : public ResourceManager< Texture >
{
public:

    static auto & Instance()
    {
        static TextureResourceManager instance;
        return instance;
    }

    virtual bool CreateResourceFromJson( std::shared_ptr< Texture > & texture_ptr, const json & texture_json ) const
    {
        for ( json::const_iterator ite = texture_json.begin(); ite != texture_json.end(); ++ite )
        {
            const auto texture_type = ite.key();

            if ( texture_type == "constantcolor" )
            {
                Vec3 color = ite.value()[ "color" ];
                texture_ptr = std::make_shared< TextureConstantColor >( color );

                return true;
            }
            if ( texture_type == "checker" )
            {
                Vec3 color1 = ite.value()[ "color1" ];
                Vec3 color2 = ite.value()[ "color2" ];
                float size = ite.value()[ "squareSize" ];

                texture_ptr = std::make_shared< TextureChecker >( color1, color2, size );

                return true;
            }
        }

        return false;
    }
};

class MaterialResourceManager : public ResourceManager< Material >
{
public:

    static auto & Instance()
    {
        static MaterialResourceManager instance;
        return instance;
    }

    virtual bool CreateResourceFromJson( std::shared_ptr< Material > & material_ptr, const json & material_json ) const override
    {
        for ( json::const_iterator ite = material_json.begin(); ite != material_json.end(); ++ite )
        {
            const auto material_type = ite.key();
            
            std::shared_ptr< Texture > texture_ptr;
            if ( !TryGetTexture( texture_ptr, ite.value() ) )
            {
                return false;
            }

            if ( material_type == "lambert" )
            {
                material_ptr = std::make_shared< MaterialLambert >( texture_ptr );

                return true;
            }
            if ( material_type == "metal" )
            {
                float roughness = ite.value()[ "roughness" ];
                material_ptr = std::make_shared< MaterialMetal >( texture_ptr, roughness );

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

private:

    bool TryGetTexture( std::shared_ptr< Texture > & texture_ptr, const json & material_json ) const
    {
        const auto texture_ite = material_json.find( "texture" );

        if ( texture_ite != material_json.end() )
        {
            const auto & texture_json = *texture_ite;

            if ( texture_json.is_object() )
            {
                return TextureResourceManager::Instance().CreateResourceFromJson( texture_ptr, texture_json );
            }

            return TextureResourceManager::Instance().Get( texture_ptr, texture_json.get< std::string >() );
        }

        // return true to not fail the process. some materials dont need a texture
        return true;
    }
};