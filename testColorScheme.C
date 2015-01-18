// color scheme:
// kGreen+2 
// kSpring+1
// kAzure-9
// kAzure+4

#include "TH1F.h"
#include "THStack.h"

void testColorScheme(){
	
	TH1F* a = new TH1F("a","a",10,0,10);
	a->SetFillColor( kGreen+2 );
	a->SetLineColor( 1 ) ;
	a->SetLineWidth( 2 ) ;
	TH1F* b = new TH1F("b","b",10,0,10);
	b->SetFillColor( kSpring+1 );
	b->SetLineColor( 1 ) ;
	b->SetLineWidth( 2 ) ;
	TH1F* c = new TH1F("c","c",10,0,10);
	c->SetFillColor( kAzure-9 );
	c->SetLineColor( 1 ) ;
	c->SetLineWidth( 2 ) ;
	TH1F* d = new TH1F("d","d",10,0,10);
	d->SetFillColor( kAzure+4 );
	d->SetLineColor( 1 ) ;
	d->SetLineWidth( 2 ) ;
	TH1F* e = new TH1F("e","e",10,0,10);
	e->SetFillColor( kOrange+9 );
	e->SetLineColor( 1 ) ;
	e->SetLineWidth( 2 ) ;

	for( int i = 0 ; i<10 ; i++){
		a->SetBinContent(i,10-i);
		b->SetBinContent(i,10-i);
		c->SetBinContent(i,10-i);
		d->SetBinContent(i,10-i);
		e->SetBinContent(i,10-i);
	}

	THStack* stack = new THStack("stack","stack");
	stack->Add(a);
	stack->Add(b);
	stack->Add(c);
	stack->Add(d);
	stack->Add(e);

	stack->Draw();

	TLegend* leg = new TLegend(.7,.9,.85,.7);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->AddEntry(a,"QCD","f");
	leg->AddEntry(b,"Z+jets","f");
	leg->AddEntry(c,"W+jets","f");
	leg->AddEntry(d,"t#bar{t}","f");
	leg->AddEntry(e,"signal","f");
	leg->Draw();
}