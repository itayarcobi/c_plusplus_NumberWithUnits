#pragma once
#include <string>
#include <iostream>
#include<unordered_map>
#include <fstream>
static std::unordered_map<std::string,std::unordered_map<std::string,double>> unitsconv;
namespace ariel {
        class NumberWithUnits{
            // static std::unordered_map<std::string,std::unordered_map<std::string,double>> unitsconv;
            double num;
            std::string unit;
            public:
            NumberWithUnits(double num, const std::string& unit);
            ~NumberWithUnits();
        
            static void read_units(std::ifstream& stream);
            static double conversion(const std::string& converter,const std::string& converted);
            static void Compatibility_error(const std::string& converter,const std::string& converted);
            NumberWithUnits operator +(const NumberWithUnits& other) const;
            NumberWithUnits& operator += (const NumberWithUnits& other);
            
            //onary
            NumberWithUnits operator +() const;
            
            NumberWithUnits operator -(const NumberWithUnits& other) const;
            
            NumberWithUnits& operator -=(const NumberWithUnits& other) ;
            
            //onary
            NumberWithUnits operator -() const;
            

            bool operator > (const NumberWithUnits& other)const;

            
            bool operator >= (const NumberWithUnits& other)const;

            
            bool operator < (const NumberWithUnits& other)const;

            
            bool operator <= (const NumberWithUnits& other)const;

            
            bool  operator == (const NumberWithUnits& other)const;

            
            bool  operator != (const NumberWithUnits& other)const;

            
            //perfix increment:
            NumberWithUnits& operator ++();

            
            //postfix:
            NumberWithUnits operator ++(int dummy_flag_for_postfix_increment);
            
            //perfix increment:
            NumberWithUnits& operator --();

            
            //postfix:
            NumberWithUnits operator --(int dummy_flag_for_postfix_increment);
            
            friend  NumberWithUnits operator *(const NumberWithUnits& unit_num, double num);
            friend  NumberWithUnits operator *(double num, const NumberWithUnits& unit_num);


            friend std::ostream& operator << (std::ostream& output, const NumberWithUnits& unitnum);
            friend std::istream& operator >>(std::istream& input, NumberWithUnits& unitnum);

           
        };
}