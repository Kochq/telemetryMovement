<?php
include "aux.php";

$posiciones = [];

$seMueveE = file("https://relevar.com.ar/datos/riego/011/reportes/04-02-2024/Movil023_E.txt");
$seMueveC = file("https://relevar.com.ar/datos/riego/011/reportes/04-02-2024/Movil023_C.txt");
$noSeMueveE = file("https://relevar.com.ar/datos/riego/042/reportes/18-03-2024/Movil002_E.txt");
$noSeMueveC = file("https://relevar.com.ar/datos/riego/042/reportes/18-03-2024/Movil002_C.txt");

$dir = 0;

for($i = 0; $i < count($noSeMueveE); $i++) {
    $datosE = explode(",", $noSeMueveE[$i]);
    $datosC = explode(",", $noSeMueveC[$i]);

    $latLng = new StdClass();

    $latLng->latE = $datosE[1];
    $latLng->lngE = $datosE[3];
    $latLng->latC = $datosC[1];
    $latLng->lngC = $datosC[3];

    $posicion = new StdClass();

    $posicion->latLng = $latLng;
    $posicion->azimuth = Calculo_AZIMUTH($latLng->latC, $latLng->lngC, $latLng->latE, $latLng->lngE);

    if(!isset($posiciones[0])) {
        $posiciones[0] = $posicion;
        continue;
    }

    $lastPosicion = end($posiciones);

    if(count($posiciones) == 1) {
        if($posicion->azimuth > $lastPosicion->azimuth) {
            $posiciones[] = $posicion;
            $dir = 1;
        } else if($posicion->azimuth < $lastPosicion->azimuth){
            $posiciones[] = $posicion;
            $dir = 0;
        } else {
            continue;
        }

        continue;
    }

    if($posicion->azimuth > $lastPosicion->azimuth) {
        if($dir == 1) {
            $posiciones[] = $posicion;
        } else {
            echo "Reset 1 $dir $posicion->azimuth $lastPosicion->azimuth \n";
            $posiciones = [];
        }
    } else if($posicion->azimuth < $lastPosicion->azimuth) {
        if($dir == 0) {
            $posiciones[] = $posicion;
        } else {
            echo "Reset 2 $dir $posicion->azimuth $lastPosicion->azimuth \n";
            $posiciones = [];
        }
    } else {
        continue;
    }

    if(count($posiciones) > 5) {
        echo "SE MUEVE A $dir\n";
    } else {
        echo "No se mueve\n";
    }
}
?>
