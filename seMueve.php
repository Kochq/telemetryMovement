<?php
include "aux.php";

$posiciones = [];

$seMueveE = file("https://relevar.com.ar/datos/riego/011/reportes/06-02-2024/Movil023_E.txt");
$seMueveC = file("https://relevar.com.ar/datos/riego/011/reportes/06-02-2024/Movil023_C.txt");
$noSeMueveE = file("https://relevar.com.ar/datos/riego/027/reportes/02-02-2024/Movil020_E.txt");
$noSeMueveC = file("https://relevar.com.ar/datos/riego/027/reportes/02-02-2024/Movil020_C.txt");

$dir = 0;

echo Calculo_AZIMUTH(procesar_pos("3442.3258", "W"), procesar_pos("06011.2272", "W"), procesar_pos("3442.1444", "W"),procesar_pos("06011.5048", "W"));

/* for($i = 0; $i < count($seMueveE); $i++) { */
/*     $datosE = explode(",", $seMueveE[$i]); */
/*     $datosC = explode(",", $seMueveC[$i]); */
/*  */
/*     $azimuthCalculado = $datosC[6]; */
/*  */
/*     $latLng = new StdClass(); */
/*  */
/*     $latLng->latE = procesar_pos($datosE[1], $datosE[2]); */
/*     $latLng->lngE = procesar_pos($datosE[3], $datosE[4]); */
/*     $latLng->latC = procesar_pos($datosC[1], $datosC[2]); */
/*     $latLng->lngC = procesar_pos($datosC[3], $datosC[4]); */
/*  */
/*     $posicion = new StdClass(); */
/*  */
/*     $posicion->latLng = $latLng; */
/*     $posicion->azimuth = Calculo_AZIMUTH($latLng->latC, $latLng->lngC, $latLng->latE, $latLng->lngE); */
/*  */
/*     if(!isset($posiciones[0])) { */
/*         $posiciones[0] = $posicion; */
/*         continue; */
/*     } */
/*  */
/*     $lastPosicion = end($posiciones); */
/*  */
/*     if(count($posiciones) == 1) { */
/*         if($posicion->azimuth > $lastPosicion->azimuth) { */
/*             $posiciones[] = $posicion; */
/*             $dir = 1; */
/*         } else if($posicion->azimuth < $lastPosicion->azimuth){ */
/*             $posiciones[] = $posicion; */
/*             $dir = 0; */
/*         } else { */
/*             continue; */
/*         } */
/*  */
/*         continue; */
/*     } */
/*  */
/*     if($posicion->azimuth > $lastPosicion->azimuth) { */
/*         if($dir == 1) { */
/*             $posiciones[] = $posicion; */
/*         } else { */
/*             echo "Reset 1 $dir $posicion->azimuth $lastPosicion->azimuth \n"; */
/*             $posiciones = []; */
/*         } */
/*     } else if($posicion->azimuth < $lastPosicion->azimuth) { */
/*         if($dir == 0) { */
/*             $posiciones[] = $posicion; */
/*         } else { */
/*             echo "Reset 2 $dir $posicion->azimuth $lastPosicion->azimuth \n"; */
/*             $posiciones = []; */
/*         } */
/*     } else { */
/*         continue; */
/*     } */
/*  */
/*     $diferencia = $posicion->azimuth - $azimuthCalculado; */
/*     echo "PHP: $posicion->azimuth C: $azimuthCalculado Diferencia $diferencia\n"; */
/*  */
/*     /* if(count($posiciones) > 5) { */
/*     /*     echo "SE MUEVE A $dir $i\n"; */ 
/*     /* } else { */ 
/*     /*     echo "No se mueve\n"; */ 
/*     /* } */ 
/* } */
?>
