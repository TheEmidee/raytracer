#include "hitable.h"

Hitable::Hitable( std::shared_ptr< const Material > material_ )
    : material( material_ )
{
}

//Hitable::Hitable( const json & material_json )
//    : material( CreateMaterialFromJson( material_json ) )
//{
//}