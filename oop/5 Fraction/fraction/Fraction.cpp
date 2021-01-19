#include<iostream>
#include<cstdio>
#include<cmath>
#include<sstream>
#include<string.h>
#include<vector>
#include"Fraction.h"
using namespace std;

/*Create a fration with numerator and denominator are both zero*/
Fraction::Fraction() {
	this->numerator = 0;
	this->denominator = 0;
}

/*Delete the class*/
Fraction::~Fraction() {

}

/*get the greatest common divisor of numerator and denominator of the fraction*/
long Fraction::common_divisor(long a, long b) {
	if (b == 0)return a;
	else
		return this->common_divisor(b, a%b);
}

/*get the numerator of the fraction*/
long Fraction::getNumerator() {
	return this->numerator;
}

/*get the denominator of the fraction*/
long Fraction::getDenominator() {
	return this->denominator;
}

/*set the numerator of the fraction*/
void Fraction::setNumerator(long n) {
	this->numerator = n;
}

/*set the denominator of the fraction*/
void Fraction::setDenominator(long d) {
	this->denominator = d;
}

/*simplify the numerator and the denominator of the fraction*/
bool Fraction::simplify() {
	long gcd = this->common_divisor(this->numerator, this->denominator);
	if (gcd > 1) {
		this->numerator = this->numerator / gcd;
		this->denominator = this->denominator / gcd;
		return true;
	}
	else {
		return false;
	}
}

/*Convert floating-point numbers to fractions*/
void Fraction::double2fraction(std::string input) {
	this->setDenominator(1);
	double a, b;
	int n;
	size_t p = input.find(".");
	if (p != string::npos) {
		try {
			a = atol(input.substr(0, p).c_str());
			n = input.size() - p-1;
			b = atol(input.substr(p + 1).c_str());
		}
		catch (exception& e) {
			system("cls");
			cout << e.what();
			return;
		}
	}
	this->numerator = pow(10, (double)n)*a + b;
	this->denominator = pow(10, (double)n);

	this->simplify();
}

/*Convert fractions to floating-point numbers*/
double Fraction::fraction2double() {
	double ans;
	ans = (double)this->numerator / this->denominator;
	return ans;
}

/*Convert string to fraction*/
bool Fraction::string2fraction(string input) {
	size_t p = input.find("/");
	if (p != string::npos) {
		try {
			this->numerator = atol(input.substr(0, p).c_str());
			this->denominator = atol(input.substr(p + 1).c_str());
		}
		catch (...) {
			return false;
		}
		if (this->denominator == 0)return false;
		else return true;
	}
	else {
		return false;
	}
}

/*Redefining the < of the fraction class*/
bool Fraction::operator<(Fraction input) {
	if (this->numerator*input.getDenominator() < this->denominator*input.getNumerator())
		return true;
	else
		return false;
}

/*Redefining the <= of the fraction class*/
bool Fraction::operator<=(Fraction input) {
	if (this->numerator*input.getDenominator() <= this->denominator*input.getNumerator())
		return true;
	else
		return false;
}

/*Redefining the > of the fraction class*/
bool Fraction::operator>(Fraction input) {
	if (this->numerator*input.getDenominator() > this->denominator*input.getNumerator())
		return true;
	else
		return false;
}

/*Redefining the >= of the fraction class*/
bool Fraction::operator>=(Fraction input) {
	if (this->numerator*input.getDenominator() >= this->denominator*input.getNumerator())
		return true;
	else
		return false;
}

/*Redefining the == of the fraction class*/
bool Fraction::operator==(Fraction input) {
	if (this->numerator*input.getDenominator() == this->denominator*input.getNumerator())
		return true;
	else
		return false;
}

/*Redefining the != of the fraction class*/
bool Fraction::operator!=(Fraction input) {
	if (this->numerator*input.getDenominator() != this->denominator*input.getNumerator())
		return true;
	else
		return false;
}

/*Redefining the % of the fraction class*/
long Fraction::operator%(Fraction input) {
	long ans;
	ans = ((this->numerator*input.getDenominator()) % (this->denominator*input.getNumerator())) / (this->denominator*input.getDenominator());
	return ans;
}

/*Output fraction in double*/
Fraction::operator double() {
	return this->fraction2double();
}

/*Output fraction in float*/
Fraction::operator float() {
	return (float)this->fraction2double();
}

/*Output fraction in long*/
Fraction::operator long() {
	return (long)this->fraction2double();
}

/*Output the simplified fraction in string*/
Fraction::operator std::string() {
	stringstream output;
	this->simplify();
	if (this->denominator == 1) {
		output << this->getNumerator();
	}
	else
		output << this->getNumerator() << "/" << this->getDenominator();
	return output.str();
}

/*Redefining the + of the fraction class*/
Fraction Fraction::operator+(Fraction input) {
	Fraction ans;
	if (this->denominator == input.getDenominator()) {
		ans.setDenominator(this->denominator);
		ans.setNumerator(this->numerator + input.getNumerator());
	}
	else {
		ans.setDenominator(this->denominator*input.getDenominator());
		ans.setNumerator(this->denominator*input.getNumerator() + this->numerator*input.getDenominator());
	}
	return ans;
}

/*Redefining the += of the fraction class*/
Fraction Fraction::operator+=(Fraction input) {
	if (this->denominator == input.getDenominator()) {
		this->numerator += input.getNumerator();
	}
	else {
		this->numerator = this->denominator*input.getNumerator() + this->numerator*input.getDenominator();
		this->denominator = this->denominator*input.getDenominator();
	}
	return *this;

}

/*Redefining the -= of the fraction class*/
Fraction Fraction::operator-=(Fraction input) {
	if (this->denominator == input.getDenominator()) {
		this->numerator -= input.getNumerator();
	}
	else {
		this->numerator = this->denominator*input.getNumerator()- this->numerator*input.getDenominator();
		this->denominator = this->denominator*input.getDenominator();
	}
	return *this;
}

/*Redefining the - of the fraction class*/
Fraction Fraction::operator-(Fraction input) {
	Fraction ans;
	if (this->denominator == input.getDenominator()) {
		ans.setDenominator(this->denominator);
		ans.setNumerator(this->numerator - input.getNumerator());
	}
	else {
		ans.setDenominator(this->denominator*input.getDenominator());
		ans.setNumerator(-this->denominator*input.getNumerator() + this->numerator*input.getDenominator());
	}
	return ans;
}

/*Redefining the * of the fraction class*/
Fraction Fraction::operator*(Fraction input) {
	Fraction ans;
	ans.setNumerator(this->numerator*input.getNumerator());
	ans.setDenominator(this->denominator*input.getDenominator());
	return ans;
}

/*Redefining the *= of the fraction class*/
Fraction Fraction::operator*=(Fraction input) {
	this->denominator *= input.getDenominator();
	this->numerator *= input.getNumerator();
	return *this;
}

/*Redefining the / of the fraction class*/
Fraction Fraction::operator/(Fraction input) {
	Fraction ans;
	ans.setDenominator(this->denominator*input.getNumerator());
	ans.setNumerator(this->numerator*input.getDenominator());
	return ans;
}

/*Redefining the /= of the fraction class*/
Fraction Fraction::operator/=(Fraction input) {
	this->denominator *= input.getNumerator();
	this->numerator *= input.getDenominator();
	return *this;
}

/*Redefining the ++ of the fraction class*/
Fraction Fraction::operator++() {
	this->numerator += 1;
	return *this;
}
/*Redefining the -- of the fraction class*/
Fraction Fraction::operator--() {
	this->numerator -= 1;
	return *this;
}

/*Redefining the cout<< of the fraction class*/
std::ostream& operator<<(std::ostream &out, Fraction &Fraction) {
	
	out << Fraction.getNumerator() << "/" << Fraction.getDenominator();
	return out;
}

/*Redefining the cin>> of the fraction class*/
std::istream& operator>>(std::istream &in, Fraction &Fraction) {
	string input;
	in >> input;
	bool valid = Fraction.string2fraction(input);
	if (!valid) {
		throw FractionInputFailException();
	}
	return in;
}