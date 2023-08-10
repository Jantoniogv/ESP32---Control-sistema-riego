#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "Arduino.h"

// Estados de las electrovalvulas
#define ON "ON"

#define OFF "OFF"

// Topic recibidos
const char power_motor[] = "motor/galo/bajo";

const char evDepGaloBajo[] = "deposito/galo/bajo";
const char evDepGaloBajoSec1[] = "regar/galo/bajo/sec1";
const char evDepGaloBajoSec2[] = "regar/galo/bajo/sec2";

const char evDepHuerto[] = "deposito/huerto";
const char evDepHuertoSec1[] = "regar/huerto/sec1";
const char evDepHuertoSec2[] = "regar/huerto/sec2";

const char evCasa[] = "deposito/casa";

const char dev_fan[] = "ventilador/pozo";

const char restart_control_sistema_riego[] = "control_sistema_riego/restart";
const char restart_pozo_galo_bajo[] = "pozo_galo_bajo/restart";
const char restart_valvulas_galo_bajo[] = "valvulas_galo_bajo/restart";
const char restart_nivel_dep_galo_bajo[] = "nivel_dep_galo_bajo/restart";

// Topic enviar estados
const char power_motor_state[] = "motor/galo/bajo/state";

const char evDepGaloBajoState[] = "deposito/galo/bajo/state";
const char evDepGaloBajoSec1State[] = "regar/galo/bajo/sec1/state";
const char evDepGaloBajoSec2State[] = "regar/galo/bajo/sec2/state";

const char evDepHuertoState[] = "deposito/huerto/state";
const char evDepHuertoSec1State[] = "regar/huerto/sec1/state";
const char evDepHuertoSec2State[] = "regar/huerto/sec2/state";

const char evCasaState[] = "deposito/casa/state";

const char dev_fan_state[] = "ventilador/pozo/state";

// Topic enviar niveles
const char nivelDepGaloBajo[] = "nivel/deposito/galo/bajo";
const char nivelDepHuerto[] = "nivel/deposito/huerto";

// Topic enviar corriente consumida motor
const char intensidadMotor[] = "intensidad/motor";

// Topic enviar respuesta a peticion de reinicio y log de los dispositivos comunicados con este
const char restart_control_sistema_riego_state[] = "control_sistema_riego/restart/state";
const char restart_pozo_galo_bajo_state[] = "pozo_galo_bajo/restart/state";
const char restart_valvulas_galo_bajo_state[] = "valvulas_galo_bajo/restart/state";
const char restart_nivel_dep_galo_bajo_state[] = "nivel_dep_galo_bajo/restart/state";

#endif // _DEVICE_H_