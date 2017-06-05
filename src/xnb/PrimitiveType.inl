namespace xnb
{
    template< typename T >
    std::string PrimitiveData< T >::toString() const
    {
        return util::toString( value );
    }
    
    template< typename T >
    std::unique_ptr< Data > PrimitiveTypeReader< T >::read( const File& file, std::istream& in, const std::string& fullDecl )
    {
        std::unique_ptr< Data > data( new PrimitiveData< T >() );
        PrimitiveData< T >* prim = static_cast< PrimitiveData< T >* >( data.get() );
        
        in.read( reinterpret_cast< char* >( &prim->value ), sizeof( T ) );
        
        return std::move( data );
    }
    
    template< typename T >
    bool PrimitiveTypeReader< T >::resultIsValueType() const
    {
        return true;
    }
}
