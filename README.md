# Detección de errores en bases de datos químicas

## Resumen

El desarrollo de la informática y la aplicación de sus métodos al mundo de la química ha propiciado grandes avances en esta ciencia. Sin embargo, un campo poco explorado dentro de la química computacional es la organometálica, de manera que muchas de las herramientas existentes no están preparadas para trabajar con ella. En esta área, para los investigadores es importante codificar las moléculas en representaciones lineales como SMILES y usar las herramientas software, para entre otras cosas, representar los compuestos gráficamente y comprenderlos mejor. La heterogeneidad en las distintas bases de datos públicas a la hora de consultar un mismo compuesto entorpece el trabajo de los investigadores.

Tras un análisis de esas inconsistencias, este proyecto propone una nomenclatura canónica para moléculas organometálicas en donde se le dé la suficiente importancia al metal, de manera que quede colocado el primero en el SMILES canónico resultado. En química de coordinación y organometálica, las reacciones y los compuestos no se explican bien siguiendo la teoría del enlace de valencia. Por tanto, se presentan también algunos cambios en el sistema de representación 2D de la herramienta utilizada, para ilustrar de una manera más adecuada los enlaces metal-ligando más habituales en compuestos organometálicos, las estructuras de ciclopentadienilo (Cp). Tras la validación de los resultados con diversos tests de consistencia, se obtienen finalmente resultados satisfactorios.


## Estructura del repositorio
```
├── InformeReuniones -> documento con los informes redactados tras cada reunión con la tutora durante el proyecto
├── code -> código implementado 
├── memoria -> documentación del proyecto, tanto en latex para compilado manual como en pdf 
└── output -> 
    ├── depictions -> ficheros .svg con las representaciones 2D para las moléculas del dataset
    ├── smiles -> ficheros .smi con los SMILES canónicos obtenidos
└── tests -> resultados de los tests
    ├── data -> datos de entrada para los tests
├── SMILES_strings.xlxs -> fichero excel con el dataset de moléculas
├── complete_dataset.smi -> dataset de moléculas en formato .smi usado para la obtención de los resultados
├── link_to_colab.txt -> enlace al notebook de Google Colab con las pruebas iniciales
```
