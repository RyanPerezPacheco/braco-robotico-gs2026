// ============================================================
//  Garra Robótica de 2 Dedos — Docking & Retrieval
//  Global Solution 2026 · FIAP · Project-Based Maker Lab
//  Software: OpenSCAD 2021.01
// ============================================================
//  Descrição: Garra de pinça com dois dedos articulados,
//  acionada por servo motor SG90 (9g).
//  O servo gira o eixo central, que empurra os dois braços
//  da pinça para abrir (120°) ou fechar (20°).
//
//  Pressione F5 para visualizar, F6 para renderizar,
//  depois File → Export → Export as STL.
// ============================================================

// ------------------------------------------------------------
//  PARÂMETROS — altere aqui para redimensionar
// ------------------------------------------------------------

// Base / suporte central
base_raio        = 14;   // mm — raio da base circular
base_espessura   =  6;   // mm — espessura da base

// Eixo do servo no centro
eixo_raio        =  2.5; // mm — furo para o eixo do servo SG90
eixo_altura      =  8;   // mm — altura do cubo de encaixe do horn

// Braço de cada dedo
braco_comprimento = 38;  // mm — comprimento total do dedo
braco_largura     =  8;  // mm — largura do dedo
braco_espessura   =  5;  // mm — espessura do dedo

// Curvatura do dedo (simula o arco da pinça)
curva_angulo     = 28;   // graus — curvatura interna do dedo

// Ponta do dedo (bico de captura)
ponta_comprimento = 12;  // mm
ponta_largura     =  5;  // mm
ponta_espessura   =  4;  // mm

// Abertura da pinça (ângulo entre os dois dedos)
// 0 = totalmente fechada | 40 = aberta
abertura         = 20;   // graus — ajuste para visualizar aberta/fechada

// Pino de articulação dos dedos
pino_raio        =  2;   // mm
pino_altura      =  7;   // mm

// ------------------------------------------------------------
//  MONTAGEM COMPLETA
// ------------------------------------------------------------

garra_completa();

module garra_completa() {
    // Base central com encaixe do servo
    base_servo();

    // Dedo esquerdo — espelhado
    translate([0, 0, base_espessura])
    rotate([0, 0,  abertura])
        dedo_completo();

    // Dedo direito
    translate([0, 0, base_espessura])
    rotate([0, 0, 180 - abertura])
    mirror([1, 0, 0])
        dedo_completo();
}

// ------------------------------------------------------------
module base_servo() {
    difference() {
        union() {
            // Disco base
            cylinder(h = base_espessura, r = base_raio, $fn = 64);

            // Cubo de encaixe do horn do servo (topo)
            translate([-5, -5, base_espessura - 0.1])
                cube([10, 10, eixo_altura]);

            // Reforços laterais
            translate([-base_raio, -2, 0])
                cube([base_raio * 2, 4, base_espessura]);
        }

        // Furo central para o eixo do servo
        translate([0, 0, -0.1])
            cylinder(h = base_espessura + eixo_altura + 0.2,
                     r = eixo_raio, $fn = 32);

        // Furos dos pinos de articulação (esq e dir)
        translate([ base_raio - 4, 0, -0.1])
            cylinder(h = base_espessura + 0.2, r = pino_raio, $fn = 24);
        translate([-(base_raio - 4), 0, -0.1])
            cylinder(h = base_espessura + 0.2, r = pino_raio, $fn = 24);

        // Aliviamento de material (visual espacial)
        for(a = [45, 135, 225, 315])
            rotate([0, 0, a])
                translate([7, 0, -0.1])
                    cylinder(h = base_espessura + 0.2, r = 2.5, $fn = 20);
    }

    // Pinos de articulação (sobressaem da base)
    translate([ base_raio - 4, 0, base_espessura])
        cylinder(h = pino_altura, r = pino_raio - 0.3, $fn = 24);
    translate([-(base_raio - 4), 0, base_espessura])
        cylinder(h = pino_altura, r = pino_raio - 0.3, $fn = 24);
}

// ------------------------------------------------------------
module dedo_completo() {
    // Posiciona o dedo a partir do pino de articulação
    translate([-(base_raio - 4), 0, 0]) {
        difference() {
            union() {
                // Segmento 1 — raiz do dedo (horizontal)
                hull() {
                    cylinder(h = braco_espessura,
                             r = braco_largura / 2, $fn = 32);
                    translate([braco_comprimento * 0.45, 0, 0])
                        cylinder(h = braco_espessura,
                                 r = braco_largura / 2 - 1, $fn = 32);
                }

                // Segmento 2 — cotovelo curvado para dentro
                translate([braco_comprimento * 0.45, 0, 0])
                rotate([0, 0, -curva_angulo])
                hull() {
                    cylinder(h = braco_espessura,
                             r = braco_largura / 2 - 1, $fn = 32);
                    translate([braco_comprimento * 0.35, 0, 0])
                        cylinder(h = braco_espessura,
                                 r = braco_largura / 2 - 2, $fn = 32);
                }

                // Ponta / bico de captura
                translate([braco_comprimento * 0.45, 0, 0])
                rotate([0, 0, -curva_angulo])
                translate([braco_comprimento * 0.35, 0, 0])
                rotate([0, 0, -curva_angulo * 0.5])
                hull() {
                    cylinder(h = braco_espessura,
                             r = braco_largura / 2 - 2, $fn = 32);
                    translate([ponta_comprimento, 0, 0])
                        cylinder(h = ponta_espessura,
                                 r = 1.5, $fn = 20);
                }
            }

            // Furo do pino de articulação na raiz
            translate([0, 0, -0.1])
                cylinder(h = braco_espessura + 0.2,
                         r = pino_raio, $fn = 24);

            // Ranhura interna para aderência (textura anti-deslizamento)
            for(i = [0.3, 0.5, 0.7])
                translate([braco_comprimento * i * 0.7,
                           -(braco_largura / 2 - 1), braco_espessura / 2])
                rotate([0, 90, 90])
                    cylinder(h = braco_largura, r = 0.8,
                             center = true, $fn = 16);
        }
    }
}
