#ifndef FRACTION_H
#define FRACTION_H
class Fraction {
public:
	Fraction(void);			//Create a fration with numerator and denominator are both zero
	~Fraction();			//Delete the class

private:
	long numerator, denominator;

public:
	long common_divisor(long a, long b);	//get the greatest common divisor of numerator and denominator of the fraction
	long getNumerator();					//get the numerator of the fraction
	long getDenominator();					//get the denominator of the fraction
	void setNumerator(long);				//set the numerator of the fraction
	void setDenominator(long);				//set the denominator of the fraction
	bool simplify();						//simplify the numerator and the denominator of the fraction
	void double2fraction(std::string);			//Convert floating-point numbers to fractions
	double fraction2double();				//Convert fractions to floating-point numbers
	bool string2fraction(std::string);		//Convert string to fraction
	bool operator<(Fraction);				//Redefining the < of the fraction class
	bool operator<=(Fraction);				//Redefining the <= of the fraction class
	bool operator>(Fraction);				//Redefining the > of the fraction class
	bool operator>=(Fraction);				//Redefining the >= of the fraction class
	bool operator==(Fraction);				//Redefining the == of the fraction class
	bool operator!=(Fraction);				//Redefining the != of the fraction class
	long operator%(Fraction);				//Redefining the % of the fraction class
	operator double();						//Output fraction in double
	operator float();						//Output fraction in float
	operator long();						//Output fraction in long
	operator std::string();					//Output the simplified fraction in string
	Fraction operator+(Fraction);			//Redefining the + of the fraction class
	Fraction operator+=(Fraction);			//Redefining the += of the fraction class
	Fraction operator-=(Fraction);			//Redefining the -= of the fraction class
	Fraction operator-(Fraction);			//Redefining the - of the fraction class
	Fraction operator*(Fraction);			//Redefining the * of the fraction class
	Fraction operator*=(Fraction);			//Redefining the *= of the fraction class
	Fraction operator/(Fraction);			//Redefining the / of the fraction class
	Fraction operator/=(Fraction);			//Redefining the /= of the fraction class
	Fraction operator++();					//Redefining the ++ of the fraction class
	Fraction operator--();					//Redefining the -- of the fraction class
};

class FractionInputFailException :public std::exception {
public:
	virtual const char* what() const throw() {
		return "Incorrect Input!!!";
	}
};

/** Left Shift Operator overloading functions */
std::ostream& operator<<(std::ostream &out, Fraction &Fraction);
/** Right Shift Operator overloading functions */
std::istream& operator>>(std::istream &in, Fraction &Fraction);

#endif // !FRACTION_H
