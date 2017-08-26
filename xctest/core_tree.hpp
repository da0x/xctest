//C++
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  Filename     : ASCII_Table.hpp                                          //
//  Created by   : Daher Alfawares                                          //
//  Created Date : 06/20/2009                                               //
//  License      : Apache License 2.0                                       //
//                                                                          //
// Copyright 2009 Daher Alfawares                                           //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
// http://www.apache.org/licenses/LICENSE-2.0                               //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

/*
 
 ASCII Table is a C++ class that allows you to format your data in an ASCII dynamic table structure.
 
 Example:
 
 ascii::table myTable("Table Title");
 myTable ("header 1")("header 2")++;
 myTable ("value 11")("value 12")++;
 myTable ("value 21")("value 22")++;
 
 std::cout << myTable;
 
 Output:
 
 +---------------------+
 |Table Title          |
 +----------+----------+
 | header 1 | header 2 |
 +----------+----------+
 | value 11 | value 12 |
 | value 21 | value 22 |
 +----------+----------+
 
 */


#ifndef _DA_ASCII_Table__0a61eabe_d038_4d30_a6fb_e202b2dfd6a9
#define _DA_ASCII_Table__0a61eabe_d038_4d30_a6fb_e202b2dfd6a9

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <numeric>
#include <sstream>

namespace ascii
{
    // forward:
    
    // the following allows adding a C style string to an std::string and returns an std::string.
    // example:
    // std::string a = "abc";
    // std::string b = "123" + a;
    //
    
    inline std::string operator + ( const char*a, const std::string& b );
    inline std::string itoa( const int &i );
    inline int atoi( const std::string&s );
    
    // lexical cast.
    template<typename _To, typename _From> class lexical_cast;
    struct scoped_mute_s;
    // formats the output.
    std::size_t length( std::string _String);
    std::string fill(std::string _Str, std::size_t _Size);
    
    class table
    {
    public:
        std::string					title;
        size_t						column;
        std::vector<std::string>	columns;
        std::vector<std::size_t>	column_widths;
        std::string					comment;
        std::string					Prefix; // line prefix.
        
        table( std::string t ):title(t),column(0){}
        
        
        std::vector< std::vector<std::string> > rows;
        
        
        table & operator()(std::string t)
        {
            if( column >= columns.size() )
            {
                columns.push_back("");
                column_widths.push_back(0);
            }
            
            columns[column] = t;
            if( column_widths[column] < length(t) )
                column_widths[column] = length(t);
            
            ++column;
            
            return *this;
        }
        table & operator()(int i)
        {
            return (*this)(itoa(i));
        }
        
        void operator ++(int)
        {
            column = 0;
            rows.push_back( columns );
        }
        
        std::string prefix(){ return Prefix; }
        void prefix( std::string _P ){ Prefix=_P; }
        
        std::string endl(){ return "\r\n" + prefix() + " "; }
    };
    
    inline std::ostream& operator << ( std::ostream& _Str, table& T )
    {
        
        int width = std::accumulate( T.column_widths.begin(), T.column_widths.end(), 0 );
        width += 3*T.columns.size();
        
        _Str << std::setiosflags(std::ios::left);
        
        _Str << T.endl();
        
        // top border.
        _Str
        << '+';
        for( int b=0; b< width-1; b++ )
            _Str
            << '-';
        _Str
        << '+'
        << T.endl();
        
        // title.
        _Str
        << "|"
        << std::setw(width-1)
        << T.title
        << "|"
        << T.endl();
        
        // middle border
        for( size_t i=0; i< T.columns.size(); i++ )
            _Str
            << "+-"
            << std::setw( static_cast<int>(T.column_widths[i]) )
            << std::setfill('-')
            << ""
            << std::setfill(' ')
            << "-";
        _Str
        << "+"
        << T.endl();
        
        
        if( !T.rows.empty() || !T.rows[0].empty() )
        {
            size_t i=0,j=0;
            
            // first row as header.
            for( i=0; i< T.columns.size(); i++ )
            {
                _Str
                << "| "
                << fill( T.rows[j][i],  T.column_widths[i] )
                << " ";
            }
            _Str
            << "|"
            << T.endl();
            
            // middle border
            for( size_t i=0; i< T.columns.size(); i++ )
            {
                _Str
                << "+-"
                << std::setw( static_cast<int>(T.column_widths[i]) )
                << std::setfill('-')
                << ""
                << std::setfill(' ')
                << "-";
            }
            _Str
            << "+"
            << T.endl();
            
            // comment if available.
            if( !T.comment.empty() )
                _Str
                << "|"
                << std::setw(width-1)
                << T.comment
                << "|"
                << T.endl();
            
            // full table.
            for( size_t j=1; j< T.rows.size(); j++ )
            {
                for( size_t i=0; i< T.columns.size(); i++ )
                {
                    _Str
                    << "| "
                    << std::setw( static_cast<int>(T.column_widths[i]) )
                    << T.rows[j][i]
                    << " ";
                }
                _Str
                << "|"
                << T.endl();
            }
        }
        
        // bottom border.
        for( size_t i=0; i< T.columns.size(); i++ )
        {
            _Str
            << "+-"
            << std::setw( static_cast<int>(T.column_widths[i]) )
            << std::setfill('-')
            << ""
            << std::setfill(' ')
            << "-";
        }
        _Str << std::resetiosflags(std::ios::left);
        _Str
        << "+"
        << std::endl;
        
        return _Str;
    }
    
    inline std::string operator + ( const char*a, const std::string& b )
    {
        std::string c;
        c += a;
        c += b;
        return c;
    }
    
    inline std::string itoa( const int &i )
    {
        std::stringstream Str;
        Str << i;
        return Str.str();
    }
    
    inline int atoi( const std::string&s )
    {
        int i;
        std::istringstream Str(s);
        Str >> i;
        return i;
    }
    
    // lexical cast.
    template<typename _To, typename _From> class lexical_cast
    {
        _From * from;
    public:
        lexical_cast<_To, _From>( _From From )
        {
            this->from = & From;
        }
        
        operator _To()
        {
            throw( "Bad lexical cast: " );
        }
    };
    
    inline
    std::size_t length( std::string _String){
        std::size_t length = 0;
        //while (*s)
        for(auto Char:_String)
            length += (Char++ & 0xc0) != 0x80;
        return length;
    }
    
    inline
    std::string fill(std::string _Str, std::size_t _Size){
        
        if( length(_Str) < _Size ){
            std::size_t Difference = _Size - length(_Str);
            std::stringstream _Out;
            _Out << _Str;
            for(int i=0; i< Difference; i++){
                _Out << " ";
            }
            return _Out.str();
        }
        
        return _Str;
    }
    
} /* ascii */

#endif
