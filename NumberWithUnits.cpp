#include <iostream>
using namespace std;
#include "NumberWithUnits.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <queue>

namespace ariel{
    // unordered_map<string,unordered_map<string,double>> NumberWithUnits::unitsconv;
    const double TOLERANCE = 0.0001;
    NumberWithUnits::NumberWithUnits(double num,const string& unit){
        if(unitsconv.find(unit)!=unitsconv.end()){
            this->num=num;
            this->unit=unit;
        }
        else{
            throw out_of_range("NumberWithUnits Class Error: Initialized with unrecognized unit");
        }
    }
     NumberWithUnits::~NumberWithUnits(){}

         void NumberWithUnits::read_units(std::ifstream& stream){
            string fline1;
            string converter;
            string converted;
            unsigned int i=1;

            while(getline(stream,fline1)){
            std::erase(fline1,' ');
            while(fline1[i]!='='){
                        converter+=fline1[i];
                        i++;
                    }
            string fline2=fline1.substr(i+1,fline1.length());
            double conversion_rate = stod(fline2);
            i=0;
          
            while((isdigit(fline2[i])!=0) || (fline2[i]=='.')){
                i++;
            }
             while(i<fline2.length()){
                        converted+=fline2[i];
                        i++;
                    }
            unitsconv[converter][converted]=conversion_rate;
            unitsconv[converted][converter]=1/conversion_rate;
            converter="";
            converted="";
            i=1;
            }
         
        }
            double NumberWithUnits::conversion(const string& converter,const string& converted){
                queue<string> q;
                double rate=0;
                double rate_conv=0;
                double rate_prev=0;
                if(unitsconv.at(converter).find(converted)!=unitsconv.at(converter).end()){
                    rate=unitsconv.at(converter).at(converted);
                }
                else{
                    std::unordered_map<std::string,double> convertion_proccess;
                    q.push(converter);
                    convertion_proccess[converter]=1;
                     while(!q.empty()){
                        string first_queue =q.front();
                        std::unordered_map<std::string,double>::iterator it ;
                        for (it=unitsconv.at(first_queue).begin();it!=unitsconv.at(first_queue).end();it++)
                        {
                            if(it->first!=converted){
                                if(convertion_proccess.find(it->first)==convertion_proccess.end()){
                                q.push(it->first);
                                rate_conv=unitsconv.at(first_queue).at(it->first);
                                rate_prev=convertion_proccess[first_queue];
                                convertion_proccess[it->first]=rate_conv*rate_prev;
                                }
                            }
                            else
                            {
                                rate_conv=unitsconv.at(first_queue).at(it->first);
                                rate_prev=convertion_proccess[first_queue];
                                rate=rate_conv*rate_prev;
                                break;
                            }
                            }
                        q.pop();
                     } 
                }
                return rate;
            }
             void NumberWithUnits::Compatibility_error(const std::string& converter,const std::string& converted){
                if(conversion(converter,converted)==0){
                    throw out_of_range{"Units do not match - ["+converted+"] cannot be converted to ["+converter+"] "};
                }
            }    
            NumberWithUnits NumberWithUnits::operator +(const NumberWithUnits& other)const {
                Compatibility_error(this->unit,other.unit);
                double rate= conversion(this->unit,other.unit);
                double ans=this->num+(other.num/rate);
                return NumberWithUnits(ans,this->unit);
            }
            NumberWithUnits& NumberWithUnits::operator +=(const NumberWithUnits& other){
                Compatibility_error(this->unit,other.unit);
                double rate= conversion(this->unit,other.unit);
                double ans=this->num+(other.num/rate);
                this->num=ans;
                return *this;
            }

            //onary
            NumberWithUnits NumberWithUnits::operator +() const{
                return NumberWithUnits{this->num,this->unit};
            }
            NumberWithUnits NumberWithUnits::operator -(const NumberWithUnits& other) const {
                Compatibility_error(this->unit,other.unit);
                double rate= conversion(this->unit,other.unit);
                double ans=this->num-(other.num/rate);
                return NumberWithUnits(ans,this->unit);
            }
            
             NumberWithUnits& NumberWithUnits::operator -=(const NumberWithUnits& other) {
                Compatibility_error(this->unit,other.unit);
                double rate= conversion(this->unit,other.unit);
                double ans=this->num-(other.num/rate);
                this->num=ans;
                return *this;
             }
            
            //onary
            NumberWithUnits NumberWithUnits::operator -() const{
                return NumberWithUnits{-(this->num),this->unit};
            }
            

            bool NumberWithUnits::operator > (const NumberWithUnits& other)const{
                Compatibility_error(this->unit,other.unit);
                bool ans=false;
                double rate= conversion(this->unit,other.unit);
                if(this->num > (other.num/rate)){
                    ans=true;
                }
                return ans;
            }
            bool NumberWithUnits::operator >= (const NumberWithUnits& other)const{
                Compatibility_error(this->unit,other.unit);
                bool ans=false;
                double rate= conversion(this->unit,other.unit);
                if(this->num >= (other.num/rate)){
                    ans= true;
                }
                return ans;
            }
            bool NumberWithUnits::operator < (const NumberWithUnits& other)const{
                Compatibility_error(this->unit,other.unit);
                bool ans=false;
                double rate= conversion(this->unit,other.unit);
                if(this->num < (other.num/rate)-TOLERANCE){
                    ans= true;
                }
                return ans;
            }
            bool NumberWithUnits::operator <= (const NumberWithUnits& other)const{
                Compatibility_error(this->unit,other.unit);
                bool ans=false;                
                double rate= conversion(this->unit,other.unit);
                if(this->num <= (other.num/rate)){
                    ans= true;
                }
                return ans;
            }
            bool  NumberWithUnits::operator == (const NumberWithUnits& other)const{
                bool ans=false;
                if(this->num==0 && other.num==0){
                    ans=true;
                }
                else{
                Compatibility_error(this->unit,other.unit);
                double rate=1;
                if(this->unit!=other.unit){
                rate= conversion(this->unit,other.unit);
                }
                if(abs(this->num -(other.num/rate))<=TOLERANCE && this->num*other.num>=0){
                    ans= true;
                }
                }
                return ans;
            }

            bool  NumberWithUnits::operator != (const NumberWithUnits& other)const{
                Compatibility_error(this->unit,other.unit);
                return (!(*this==other));
            }
            //perfix increment:
            NumberWithUnits& NumberWithUnits::operator ++(){
                this->num=this->num+1;
                return *this;
            }

            //postfix:
            NumberWithUnits NumberWithUnits::operator ++(int dummy_flag_for_postfix_increment){
                NumberWithUnits copy {this->num,this->unit} ;
                this->num+=1;
                return copy;
                 }
            
            //perfix increment:
            NumberWithUnits& NumberWithUnits::operator --(){
                this->num=this->num-1;
                return *this;            }
            //postfix:
            NumberWithUnits NumberWithUnits::operator --(int dummy_flag_for_postfix_increment){
                NumberWithUnits copy{this->num,this->unit};
                this->num-=1;
                return copy;         
            }        
             NumberWithUnits operator *(const NumberWithUnits& unit_num, double num){
                return NumberWithUnits{unit_num.num*num,unit_num.unit} ;
            }
             NumberWithUnits operator *(double num, const NumberWithUnits& unit_num){
                return NumberWithUnits{unit_num.num*num,unit_num.unit} ;
            }
             std::ostream& operator << (std::ostream& output, const NumberWithUnits& unitnum){
                return (output << unitnum.num << '[' << unitnum.unit << ']');
             }
             static istream& getAndCheckNextCharIs(istream& input, char expectedChar) {
                char actualChar=0;
                input >> actualChar;
                if (!input) {return input;}

                if (actualChar!=expectedChar) {
        
                input.setstate(ios::failbit);
                }
                return input;
                }
             std::istream& operator >>(std::istream& input, NumberWithUnits& unitnum){
                 double num = 0;
                 string unit;

             // remember place for rewinding
                ios::pos_type startPosition = input.tellg();

                if ( (!(input >> num))                 ||
                (!getAndCheckNextCharIs(input,'['))  ||
                (!(input >> unit)) ||
                (unit.at(unit.length()-1)!=']'        &&
                (!getAndCheckNextCharIs(input,']'))))           
                {
                auto errorState = input.rdstate(); // remember error state
                input.clear(); // clear error so seekg will work
                input.seekg(startPosition); // rewind
                input.clear(errorState); // set back the error flag
                } 
                else {
                unitnum.num = num;
                if(unit[unit.length()-1]==']'){
                    unit=unit.substr(0,unit.length()-1);
                }
                //NumberWithUnits::
                if(unitsconv.find(unit)==unitsconv.end()){
                        throw out_of_range("NumberWithUnits Class Error: Initialized with unrecognized unit");
                }
                unitnum.unit = unit;
                }
                return input ;
             }
};
