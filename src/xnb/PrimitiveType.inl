namespace xnb
{
    template< typename T >
    std::string PrimitiveData< T >::toString() const
    {
        return util::toString( value );
    }
    
    template< typename T >
    std::string PrimitiveData< T >::getType() const
    {
        return "type?";
    }
    
    template<> std::string PrimitiveData< sf::  Int8 >::getType() const { return PRIMITIVE_INT8_TYPE;   }
    template<> std::string PrimitiveData< sf:: Uint8 >::getType() const { return PRIMITIVE_UINT8_TYPE;  }
    template<> std::string PrimitiveData< sf:: Int16 >::getType() const { return PRIMITIVE_INT16_TYPE;  }
    template<> std::string PrimitiveData< sf::Uint16 >::getType() const { return PRIMITIVE_UINT16_TYPE; }
    template<> std::string PrimitiveData< sf:: Int32 >::getType() const { return PRIMITIVE_INT32_TYPE;  }
    template<> std::string PrimitiveData< sf::Uint32 >::getType() const { return PRIMITIVE_UINT32_TYPE; }
    template<> std::string PrimitiveData< sf:: Int64 >::getType() const { return PRIMITIVE_INT64_TYPE;  }
    template<> std::string PrimitiveData< sf::Uint64 >::getType() const { return PRIMITIVE_UINT64_TYPE; }
    template<> std::string PrimitiveData< float  >::getType() const { return PRIMITIVE_FLOAT_TYPE;  }
    template<> std::string PrimitiveData< double >::getType() const { return PRIMITIVE_DOUBLE_TYPE; }
    template<> std::string PrimitiveData< bool   >::getType() const { return PRIMITIVE_BOOL_TYPE;   }
    
    template< typename T >
    TypeReaderHeader PrimitiveData< T >::getTypeReader() const
    {
        return TypeReaderHeader{ "typeReader?", 0 };
    }
    
    template<> TypeReaderHeader PrimitiveData< sf::  Int8 >::getTypeReader() const { return TypeReaderHeader{ PRIMITIVE_INT8_TYPEREADER   }; }
    template<> TypeReaderHeader PrimitiveData< sf:: Uint8 >::getTypeReader() const { return TypeReaderHeader{ PRIMITIVE_UINT8_TYPEREADER  }; }
    template<> TypeReaderHeader PrimitiveData< sf:: Int16 >::getTypeReader() const { return TypeReaderHeader{ PRIMITIVE_INT16_TYPEREADER  }; }
    template<> TypeReaderHeader PrimitiveData< sf::Uint16 >::getTypeReader() const { return TypeReaderHeader{ PRIMITIVE_UINT16_TYPEREADER }; }
    template<> TypeReaderHeader PrimitiveData< sf:: Int32 >::getTypeReader() const { return TypeReaderHeader{ PRIMITIVE_INT32_TYPEREADER  }; }
    template<> TypeReaderHeader PrimitiveData< sf::Uint32 >::getTypeReader() const { return TypeReaderHeader{ PRIMITIVE_UINT32_TYPEREADER }; }
    template<> TypeReaderHeader PrimitiveData< sf:: Int64 >::getTypeReader() const { return TypeReaderHeader{ PRIMITIVE_INT64_TYPEREADER  }; }
    template<> TypeReaderHeader PrimitiveData< sf::Uint64 >::getTypeReader() const { return TypeReaderHeader{ PRIMITIVE_UINT64_TYPEREADER }; }
    template<> TypeReaderHeader PrimitiveData< float  >::getTypeReader() const { return TypeReaderHeader{ PRIMITIVE_FLOAT_TYPEREADER  }; }
    template<> TypeReaderHeader PrimitiveData< double >::getTypeReader() const { return TypeReaderHeader{ PRIMITIVE_DOUBLE_TYPEREADER }; }
    template<> TypeReaderHeader PrimitiveData< bool   >::getTypeReader() const { return TypeReaderHeader{ PRIMITIVE_BOOL_TYPEREADER   }; }
    
    template< typename T >
    std::unique_ptr< Data > PrimitiveTypeReader< T >::read( const File& file, std::istream& in, const std::string& fullDecl )
    {
        std::unique_ptr< Data > data( new PrimitiveData< T >() );
        PrimitiveData< T >* prim = static_cast< PrimitiveData< T >* >( data.get() );
        
        in.read( reinterpret_cast< char* >( &prim->value ), sizeof( T ) );
        
        return std::move( data );
    }
    
    template< typename T >
    void PrimitiveTypeReader< T >::write( const File& file, std::ostream& out, const Data* data )
    {
        const PrimitiveData< T >* prim = dynamic_cast< const PrimitiveData< T >* >( data );
        if ( prim == nullptr )
            throw std::invalid_argument( "Bad data type passed to write" );
        
        out.write( reinterpret_cast< const char* >( &prim->value ), sizeof( T ) );
    }
    
    template< typename T >
    bool PrimitiveTypeReader< T >::resultIsValueType() const
    {
        return true;
    }
    
    template<> std::string PrimitiveTypeReader< sf::  Int8 >::getReaderType() const { return PRIMITIVE_INT8_TYPEREADER   ; }
    template<> std::string PrimitiveTypeReader< sf:: Uint8 >::getReaderType() const { return PRIMITIVE_UINT8_TYPEREADER  ; }
    template<> std::string PrimitiveTypeReader< sf:: Int16 >::getReaderType() const { return PRIMITIVE_INT16_TYPEREADER  ; }
    template<> std::string PrimitiveTypeReader< sf::Uint16 >::getReaderType() const { return PRIMITIVE_UINT16_TYPEREADER ; }
    template<> std::string PrimitiveTypeReader< sf:: Int32 >::getReaderType() const { return PRIMITIVE_INT32_TYPEREADER  ; }
    template<> std::string PrimitiveTypeReader< sf::Uint32 >::getReaderType() const { return PRIMITIVE_UINT32_TYPEREADER ; }
    template<> std::string PrimitiveTypeReader< sf:: Int64 >::getReaderType() const { return PRIMITIVE_INT64_TYPEREADER  ; }
    template<> std::string PrimitiveTypeReader< sf::Uint64 >::getReaderType() const { return PRIMITIVE_UINT64_TYPEREADER ; }
    template<> std::string PrimitiveTypeReader< float  >::getReaderType() const { return PRIMITIVE_FLOAT_TYPEREADER  ; }
    template<> std::string PrimitiveTypeReader< double >::getReaderType() const { return PRIMITIVE_DOUBLE_TYPEREADER ; }
    template<> std::string PrimitiveTypeReader< bool   >::getReaderType() const { return PRIMITIVE_BOOL_TYPEREADER   ; }
}
