#ifndef XNB_PRIMITIVETYPE_HPP
#define XNB_PRIMITIVETYPE_HPP

#include "xnb/Data.hpp"
#include "xnb/ITypeReader.hpp"

namespace xnb
{
    template< typename T >
    class PrimitiveData : public Data
    {
        public:
            T value;
            
            virtual std::string toString() const override;
            
            virtual std::string getType() const override;
            virtual TypeReaderHeader getTypeReader() const override;
    };
    
    template< typename T >
    class PrimitiveTypeReader : public ITypeReader
    {
        public:
            virtual std::unique_ptr< Data > read( const File& file, std::istream& in, const std::string& fullDecl ) override;
            virtual void write( const File& file, std::ostream& out, const Data* data ) override;
            
            virtual bool resultIsValueType() const override;
            virtual std::string getReaderType() const override;
    };
    
    constexpr const char* PRIMITIVE_INT8_TYPE = "System.SByte";
    constexpr const char* PRIMITIVE_INT8_TYPEREADER = "Microsoft.Xna.Framework.Content.SByteReader";
    constexpr const char* PRIMITIVE_UINT8_TYPE = "System.Byte";
    constexpr const char* PRIMITIVE_UINT8_TYPEREADER = "Microsoft.Xna.Framework.Content.ByteReader";
    constexpr const char* PRIMITIVE_INT16_TYPE = "System.Int16";
    constexpr const char* PRIMITIVE_INT16_TYPEREADER = "Microsoft.Xna.Framework.Content.Int16Reader";
    constexpr const char* PRIMITIVE_UINT16_TYPE = "System.Uint16";
    constexpr const char* PRIMITIVE_UINT16_TYPEREADER = "Microsoft.Xna.Framework.Content.Uint16Reader";
    constexpr const char* PRIMITIVE_INT32_TYPE = "System.Int32";
    constexpr const char* PRIMITIVE_INT32_TYPEREADER = "Microsoft.Xna.Framework.Content.Int32Reader";
    constexpr const char* PRIMITIVE_UINT32_TYPE = "System.Uint32";
    constexpr const char* PRIMITIVE_UINT32_TYPEREADER = "Microsoft.Xna.Framework.Content.Uint32Reader";
    constexpr const char* PRIMITIVE_INT64_TYPE = "System.Int64";
    constexpr const char* PRIMITIVE_INT64_TYPEREADER = "Microsoft.Xna.Framework.Content.Int64Reader";
    constexpr const char* PRIMITIVE_UINT64_TYPE = "System.Uint64";
    constexpr const char* PRIMITIVE_UINT64_TYPEREADER = "Microsoft.Xna.Framework.Content.Uint64Reader";
    constexpr const char* PRIMITIVE_FLOAT_TYPE = "System.Single";
    constexpr const char* PRIMITIVE_FLOAT_TYPEREADER = "Microsoft.Xna.Framework.Content.SingleReader";
    constexpr const char* PRIMITIVE_DOUBLE_TYPE = "System.Double";
    constexpr const char* PRIMITIVE_DOUBLE_TYPEREADER = "Microsoft.Xna.Framework.Content.DoubleReader";
    constexpr const char* PRIMITIVE_BOOL_TYPE = "System.Boolean";
    constexpr const char* PRIMITIVE_BOOL_TYPEREADER = "Microsoft.Xna.Framework.Content.BooleanReader";
}

#include "xnb/PrimitiveType.inl"

#endif // XNB_PRIMITIVETYPE_HPP
