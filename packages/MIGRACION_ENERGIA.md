# Migración a los packages de energía

Los ficheros `energia_*.yaml.bak` de esta carpeta **no los carga Home Assistant**
(`!include_dir_named packages` solo lee `*.yaml`). Para activarlos hay que
seguir estos pasos **en orden** y en una sola sesión, porque si se activan los
packages sin limpiar lo antiguo habrá entity_id duplicados (`_2`) y claves
repetidas de `utility_meter`.

> Antes de empezar: copia de seguridad completa de HA.

---

## 1. Activar los packages

Renombrar quitando `.bak`:

```
packages/energia_base.yaml.bak              -> energia_base.yaml
packages/energia_calculos.yaml.bak          -> energia_calculos.yaml
packages/energia_electrodomesticos.yaml.bak -> energia_electrodomesticos.yaml
packages/energia_contadores.yaml.bak        -> energia_contadores.yaml
```

## 2. Borrar de `template.yaml`

Sensores sustituidos por los packages (borrar el bloque completo de cada uno):

| Nombre en template.yaml | Sustituto |
|---|---|
| `red` | `sensor.consumo_red` |
| `excedentes` | `sensor.excedentes_red` |
| `bateria_carga` | `sensor.carga_bateria` |
| `bateria_carga_mas_mppt` | *(se elimina: fórmula errónea, cuenta doble)* |
| `bateria_descarga` | `sensor.descarga_bateria` |
| `bateria_soc` | `sensor.soc_bateria` |
| `String sureste` | `sensor.string_sureste` (mismo entity_id) |
| `String suroeste` | `sensor.string_suroeste` (mismo entity_id) |
| `Consumo termos electricos` | `sensor.potencia_termos` |
| `Energia termos electricos` | `sensor.energia_termos_total` |
| `Consumo de la casa` | `sensor.consumo_casa` |
| `Consumo solar` | `sensor.autoconsumo_solar` |
| `Consumo linea enchufes` | `sensor.potencia_enchufes_resto` |
| `Consumo de la casa medible` | `sensor.consumo_medido` |
| `Consumo de la casa no medible` | `sensor.consumo_no_medido` |

## 3. Borrar de `sensor.yaml`

Las tres `integration` (sustituidas por los contadores del Shelly EM y SmartShunt):

- `consumo_de_la_red`
- `bateria_carga_kwh`
- `bateria_descarga_kwh`

Y actualizar el filtro **"Consumo no medible filtrado"**:

```yaml
  entity_id: sensor.consumo_no_medido   # antes sensor.consumo_no_medible
```

## 4. Vaciar `utility_meter.yaml`

Todo su contenido pasa a `energia_contadores.yaml` con **los mismos nombres y
unique_id** (se conservan entity_id e historial). Dejar el fichero vacío o
quitar `utility_meter: !include utility_meter.yaml` de `configuration.yaml`.

Después, en *Ajustes → Dispositivos → Ayudantes*, borrar los helpers de UI que
ya no alimentan nada: `excedentes_kwh` y `consumo_de_la_casa_kwh`.

## 5. Actualizar referencias

**automations.yaml**

| Línea aprox. | Cambiar | Por |
|---|---|---|
| 1596 | `sensor.bateria_soc` | `sensor.soc_bateria` |
| 3362, 3406 | `sensor.consumo_de_la_casa` | `sensor.consumo_casa` |

**Dashboards**: buscar y sustituir los entity_id de la tabla del paso 2.

**Carpeta `templates/`**: no está incluida en `configuration.yaml` (código muerto).
Se puede borrar o archivar.

## 6. Recorder

`recorder.yaml` usa lista `include`: **lo que no esté en la lista no genera
estadísticas** y no se puede usar en el panel de Energía. Añadir:

```yaml
    # ===== Packages energía =====
    - input_number.capacidad_bateria_kwh
    - sensor.potencia_red
    - sensor.consumo_red
    - sensor.excedentes_red
    - sensor.energia_importada_red
    - sensor.energia_exportada_red
    - sensor.consumo_casa
    - sensor.energia_consumo_casa
    - sensor.potencia_bateria
    - sensor.carga_bateria
    - sensor.descarga_bateria
    - sensor.soc_bateria
    - sensor.energia_cargada_bateria
    - sensor.energia_descargada_bateria
    - sensor.estado_bateria
    - sensor.capacidad_restante_bateria
    - sensor.capacidad_disponible_bateria
    - sensor.potencia_fv_sma
    - sensor.potencia_fv_mppt
    - sensor.potencia_fv_total
    - sensor.energia_fv_sma_diaria
    - sensor.energia_fv_mppt_diaria
    - sensor.energia_fv_total_diaria
    - sensor.solar_mensual
    - sensor.solar_anual
    - sensor.autoconsumo_solar
    - sensor.excedente_aprovechable
    - sensor.participacion_red
    - sensor.participacion_bateria
    - sensor.participacion_solar
    - sensor.potencia_termos
    - sensor.energia_termos_total
    - sensor.potencia_enchufes_resto
    - sensor.consumo_medido
    - sensor.consumo_no_medido
    - sensor.energia_consumo_no_medido
    - sensor.energia_radiador_cocina
    - sensor.energia_radiador_salita
    - sensor.energia_lavavajillas
    - sensor.energia_lavadora
    - sensor.energia_secadora
    - sensor.energia_vitroceramica
    - sensor.energia_horno_micro_cafetera
    - sensor.energia_frigorifico
    - sensor.energia_luces_escalera
```

Y quitar de la lista los entity_id antiguos eliminados en el paso 2
(`sensor.red`, `sensor.excedentes`, `sensor.bateria_carga`, `sensor.bateria_soc`,
`sensor.consumo_termos`, `sensor.energia_termos`, `sensor.consumo_de_la_casa`,
`sensor.consumo_linea_enchufes`, `sensor.consumo_no_medible`,
`sensor.bateria_carga_kwh`).

## 7. Comprobar y reiniciar

1. *Herramientas para desarrolladores → YAML → Comprobar configuración*.
2. Reiniciar HA (no basta con recargar: hay `integration` y `utility_meter`).
3. *Ajustes → Entidades*: buscar `_2` y entidades "no disponibles"
   huérfanas de los sensores antiguos y borrarlas.

## 8. Panel de Energía

| Apartado | Sensor |
|---|---|
| Red – consumo | `sensor.energia_importada_red` |
| Red – devolución | `sensor.energia_exportada_red` |
| Solar | `sensor.energia_fv_total_diaria` (o los totales del SMA y del MPPT por separado) |
| Batería – entrada | `sensor.energia_cargada_bateria` |
| Batería – salida | `sensor.energia_descargada_bateria` |
| Dispositivos individuales | Contadores nativos (ver cabecera de `energia_electrodomesticos`) + los `sensor.energia_*` creados por integración |

Al cambiar la fuente de red/batería, el panel empieza historial nuevo para esos
apartados; el historial antiguo sigue en la base de datos.
