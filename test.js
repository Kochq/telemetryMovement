function distanciaGeodesica(lat1, long1, lat2, long2){
	let degtorad = 0.01745329; 
	let radtodeg = 57.29578; 
	
	let dlong = (long1 - long2); 
	let dvalue = (Math.sin(lat1 * degtorad)*Math.sin(lat2 * degtorad))+(Math.cos(lat1*degtorad)*Math.cos(lat2*degtorad)*Math.cos(dlong * degtorad)); 
	
	let dd = Math.acos(dvalue) * radtodeg; 
	
	let miles = (dd * 69.16); 
	let km = (dd * 109642); 
	
	return km; 
}

function Calculo_AZIMUTH(latC, lngC, latE, lngE) {
    let x42 = 0;
    
	let dist = distanciaGeodesica(latC, lngC, latE, lngE);
	let y42 = latC + (dist / 109642);
    let dist2 = distanciaGeodesica(y42, lngC, latE, lngE);   // Distancia entre el Centro_NORTE y el extremo
    
    let x2 = dist2 * dist2;
    let y2 = 2 * dist * dist;
    y42 = 1 - (x2 / y2);
	
    if ((y42 < 0) && (y42 < -1)) y42 = -1.0;
	else if(y42 > 1) y42 = 1.0;
    
	if(lngC >= lngE) {    // El Centro esta mas al Este
        if(latC > latE) x42 = 360 - (Math.acos(y42) * 57.29578); // El Centro esta mas al Norte
        else x42 = 360 - (Math.acos(y42) * 57.29578);
	}
    else if(lngC < lngE) {   // El Centro esta mas al Oeste
	   if(latC > latE) x42 = Math.acos(y42) * 57.29578;     // El Centro esta mas al Norte
       else x42 = Math.acos(y42) * 57.29578;
    }
    
    return x42;
}

console.log(Calculo_AZIMUTH(3442.3258, 6011.2272, 3442.1444, 6011.5048));
