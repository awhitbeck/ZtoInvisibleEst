
enum binning { kRA2 , k13TeV , kNumBinning };

int classicRA2(double MHT, double HT, int NJets){

  if( NJets >= 3 && NJets <= 5 ){

    if( HT > 500 && HT < 800 ){

      if( MHT > 200 && MHT < 300 ){
	return 1 ;
      }else if( MHT > 300 && MHT < 450 ){
	return 2 ; 
      }else if( MHT > 450 && MHT < 600 ){
	return 3 ; 
      }else if( MHT > 600 ){
	return 4 ;
      }

    }else if( HT > 800 && HT < 1000 ){


      if( MHT > 200 && MHT < 300 ){
	return 5 ; 
      }else if( MHT > 300 && MHT < 450 ){
	return 6 ; 
      }else if( MHT > 450 && MHT < 600 ){
	return 7 ; 
      }else if( MHT > 600 ){
	return 8 ; 
      }

    }else if( HT > 1000 && HT < 1250 ){

      if( MHT > 200 && MHT < 300 ){
	return 9 ;
      }else if( MHT > 300 && MHT < 450 ){
	return 10 ;
      }else if( MHT > 450 && MHT < 600 ){
	return 11 ; 
      }else if( MHT > 600 ){
	return 12 ;
      }

    }else if( HT > 1250 && HT < 1500 ){

      if( MHT > 200 && MHT < 300 ){
	return 13 ;
      }else if( MHT > 300 && MHT < 450 ){
	return 14 ;
      }else if( MHT > 450 ){
	return 15 ; 
      }

    }else if( HT > 1500 ){

      if( MHT > 200 && MHT < 300 ){
	return 16 ;
      }else if( MHT > 300 ){
	return 17 ; 
      }

    }
    
  }else if( NJets >= 6 && NJets <=7 ){ 
    
    if( HT > 500 && HT < 800 ){

      if( MHT > 200 && MHT < 300 ){
	return 18 ; 
      }else if( MHT > 300 && MHT < 450 ){
	return 19 ;
      }else if( MHT > 450 ){
	return 20 ;
      }

    }else if( HT > 800 && HT < 1000 ){

      if( MHT > 200 && MHT < 300 ){
	return 21 ; 
      }else if( MHT > 300 && MHT < 450 ){
	return 22 ;
      }else if( MHT > 450 ){
	return 23 ; 
      }

    }else if( HT > 1000 && HT < 1250 ){

      if( MHT > 200 && MHT < 300 ){
	return 24 ; 
      }else if( MHT > 300 && MHT < 450 ){
	return 25 ; 
      }else if( MHT > 450 ){
	return 26 ;
      }

    }else if( HT > 1250 && HT < 1500 ){

      if( MHT > 200 && MHT < 300 ){
	return 27 ; 
      }else if( MHT > 300 && MHT < 450 ){
	return 28 ; 
      }else if( MHT > 450 ){
	return 29 ; 
      }

    }else if( HT > 1500 ){

      if( MHT > 200 && MHT < 300 ){
	return 30 ;
      }else if( MHT > 300 ){
	return 31 ; 
      }

    }

  }else if( NJets >= 8 ){

    if( HT > 500 && HT < 800 ){

      if( MHT > 200 ){
	return 32 ; 
      }

    }else if( HT > 800 && HT < 1000 ){

      if( MHT > 200 ){
	return 33 ; 
      }

    }else if( HT > 1000 && HT < 1250 ){

      if( MHT > 200 ){
	return 34 ; 
      }

    }else if( HT > 1250 && HT < 1500 ){

      if( MHT > 200 ){
	return 35 ; 
      }

    }else if( HT > 1500 ){

      if( MHT > 200 ){
	return 36 ; 
      }

    }

  }

  return -1 ;

}

int scrabble(double MHT, double HT, double NJets , double BTags){

  if( MHT < 200 || HT < 500 || NJets < 4 ) return -1 ;

  int bin7s = -1;
  int bin70s = -1;

  if( MHT > 200 && MHT < 500 ){

    if( HT > 500 && HT < 800 ) 
      bin7s = 1;
    else if( HT > 800 && HT < 1200 ) 
      bin7s = 2;
    else if( HT > 1200 ) 
      bin7s = 3;
    
  }else if( MHT > 500 && MHT < 750 ){

    if( HT > 500 && HT < 1200 )
      bin7s = 4 ; 
    else if( HT > 1200 ) 
      bin7s = 5;

  }else if( MHT > 750 ){
    
    if( HT > 500 && HT < 1200 )
      bin7s = 6 ; 
    else if( HT > 1200 ) 
      bin7s = 7;

  }

  if( NJets >= 4 && NJets <= 6 ){

    if( BTags == 0 ) 
      bin70s = 0 ;
    if( BTags == 1 )
      bin70s = 1 ;
    if( BTags == 2 ) 
      bin70s = 2 ; 
    if( BTags >= 3 )
      bin70s = 3 ;

  }

  if( NJets >= 7 && NJets <= 8 ){

    if( BTags == 0 ) 
      bin70s = 4 ;
    if( BTags == 1 )
      bin70s = 5 ;
    if( BTags == 2 ) 
      bin70s = 6 ; 
    if( BTags >= 3 )
      bin70s = 7 ;

  }

  if( NJets >= 9 ){

    if( BTags == 0 ) 
      bin70s = 8 ;
    if( BTags == 1 )
      bin70s = 9 ;
    if( BTags == 2 ) 
      bin70s = 10 ; 
    if( BTags >= 3 )
      bin70s = 11 ;

  }

  return bin70s*7+bin7s ;

}



int  computeBin(double MHT, double HT, double NJets, double BTags = 0 , binning bins = kRA2){

  if( bins == kRA2 ) return classicRA2( MHT , HT , NJets ) ;  
  else if( bins == k13TeV ) return scrabble( MHT , HT , NJets , BTags ) ;  
  else { 
    cout << "computeBine() No binning found!" << endl;
    return -1; 
  }

}

