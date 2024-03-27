<?php
function distanciaGeodesica(float $lat1, float $long1, float $lat2, float $long2){
	$degtorad = 0.01745329;
	$radtodeg = 57.29578;

	$dlong = ($long1 - $long2);
    $v1 = sin($lat1 * $degtorad);
    $v2 = sin($lat2 * $degtorad);
    $v3 = cos($lat1*$degtorad);
    $v4 = cos($lat2*$degtorad);
    $v5 = cos($dlong * $degtorad);
	$dvalue = ($v1*$v2)+($v3*$v4*$v5);

	$dd = acos($dvalue) * $radtodeg;

	$km = ($dd * 109642);

	return $km;
}

function Calculo_AZIMUTH(float $latC, float $lngC, float $latE, float $lngE) {
    $x42 = 0;

	$dist = distanciaGeodesica($latC, $lngC, $latE, $lngE);
	$y42 = $latC + ($dist / 109642);
    $dist2 = distanciaGeodesica($y42, $lngC, $latE, $lngE); // Distancia entre el Centro_NORTE y el extremo

    $x2 = $dist2 * $dist2;
    $y2 = 2 * $dist * $dist;
    $y42 = 1 - ($x2 / $y2);

    if (($y42 < 0) && ($y42 < -1)) $y42 = -1.0;
	else if($y42 > 1) $y42 = 1.0;

	if($lngC >= $lngE) { // El Centro esta mas al Este
        if($latC > $latE) $x42 = 360 - (acos($y42) * 57.29578); // El Centro esta mas al Norte
        else $x42 = 360 - (acos($y42) * 57.29578);
	}
    else if($lngC < $lngE) { // El Centro esta mas al Oeste
	   if($latC > $latE) $x42 = acos($y42) * 57.29578; // El Centro esta mas al Norte
       else $x42 = acos($y42) * 57.29578;
    }

    return $x42;
}

function procesar_pos(string $lat, string $signo): float {
    if($signo == "S" || $signo = "W") {
        $signo = -1;
    } else {
        $signo = 1;
    }

    $grados = floatVal(substr($lat,strpos($lat,".") - 4 , 2));
    $minutos = floatVal(substr($lat,strpos($lat,".") - 2 , 8));
    $decimas_de_grado = $minutos / 60;

    $pos_procesada = $signo * ($grados + $decimas_de_grado);
    $pos_procesada = substr($pos_procesada, 0, 8);

    return $pos_procesada;
}
?>
