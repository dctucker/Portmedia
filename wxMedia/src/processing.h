#include <iostream>
#define extends :
#define constrain( X, Y, Z ) ((X) < (Y)) ? (Y) : ( ( (X) > (Z) ) ? (Z) : (X) )
#define CENTER wxCENTER
wxDC *maindc;

typedef bool boolean;
inline void fill(int rgb){
	maindc->SetBrush(wxColor(rgb));
} 
inline void fill(int r,int g ,int b){
	maindc->SetBrush(wxColor(r,g,b));
}
inline void rect(int x1,int y1,int x2, int y2){
	//std::cout << "rect";
	maindc->DrawRectangle(x1, y1, x2, y2);
}
inline void textFont(wxFont *f){
	maindc->SetFont(*f);
}
inline void textAlign(int a1,int a2){
}
inline void text(const wxString &str, int x,int y){
	maindc->DrawText(str, x, y);
}
inline void noStroke(){

}
inline void strokeWeight(int w){
}
inline void stroke(int rgb){
}
inline float map(float v, float m1, float M1, float m2, float M2){
	return m2 + (M2 - m2) * ( (v - m1) / (M1 - m1) );
}


