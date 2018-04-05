#pragma once

template <typename _T_ >
class Singleton
{
public:

    static _T_ & Instance() 
    {
        if ( Singleton::_instance == 0 )
        {
            Singleton::_instance = CreateInstance();
        }
        return *( Singleton::_instance );
    }

protected:

    virtual ~Singleton() 
    {
        if ( Singleton::_instance != 0 )
        {
            delete Singleton::_instance;
        }
        Singleton::_instance = 0;
    }

    inline explicit Singleton() 
    {
        assert( Singleton::_instance == 0 );
        Singleton::_instance = static_cast< _T_* >( this );
    }

private:
    static _T_ * _instance;
    static _T_ * CreateInstance() 
    {
        return new _T_();
    }
};

template<typename T>
T* Singleton<T>::_instance = 0;