# LedRGBlib - RGB LED Library for Arduino

An easy-to-use library to control RGB LEDs with different effects in Arduino. To read the Engish tutorial, [click here](#English-tutorial).
Also, if you want to contribute, [follow the instructions](#Contributing)

Librería fácil de usar en español con efectos para controlar cualquier LED RGB en Arduino. Para leer el tutorial en español, [haz clic aquí](#Tutorial-en-español).
Si deseas contribuir al proyecto, [sigue estas instrucciones](#Contribuir)

# English tutorials
## How to use it
This library is pretty easy to use, check out the following steps to set everything up.

### Initialize the library
- First, you must download and install the library [as explained in the official page from Arduino](https://www.arduino.cc/en/Guide/Libraries).
- In the first lines of your Arduino file, paste `#include <LedRGB.h>`.

### RGB LED object creation

We are using a custom object, named `LedRGB`. Next, we need to declare a new object such as `LedRGB myLED(pinRed, pinGreen, pinBlue, modeCACC);`, where the latter parameter is either `0` (or `CA`) for a Common Anode LED, or `1` (or `CC`) for a Common Cathode one.

### Functions in the library

- `setColor(redValue, greenValue, blueValue);` sets the values from 0 to 255 for each of the three colors.
- `defaultColor(colorName);` chooses among different color Strings, e.g. `"white"` or `"yellow"`. If the color you write is not supported, the LED will be set to black.
- `add(addRed, addGreen, addBlue);` adds the specified value to the current one in the LED, up to 255.
- `substract(subRed, subGreen, subBlue);` substracts the specified value, all the way down to 0.
- `off();` sets every value to 0.
- `test();` makes a test of all three colors (300ms each, then 500ms white).
- `flashEffect(timeOn, timeOff)` flashes once with the intervals specified in the parameters (milliseconds).

## Contributing
If you want to contribute to the library by adding new functions or improving the current ones, don't be afraid to create PRs with everything you would want to see! Also, if you think there are things we should improve, create a new issue.


# Tutoriales en español
## Cómo usarla
La utilización de la librería es muy sencilla, a continuación describimos los pasos necesarios.

### Inicialización de la librería
- Debes descargar e instalar la librería [tal y como aparece en esta página oficial de Arduino](https://www.arduino.cc/en/Guide/Libraries).
- En las primeras líneas de tu programa, utiliza `#include <LedRGB.h>`.

### Creación del objeto para un LED RGB

El objeto que utilizaremos será de tipo `LedRGB`. A continuación deberemos definir un objeto del tipo `LedRGB miLED(pinRed, pinGreen, pinBlue, modoCACC);`, donde el último parámetro es `0` (o `CA`) en el caso de ser un LED de Ánodo Común, y `1` (o `CC`) si es de Cátodo Común.

### Funciones de la librería

- `ponerColor(valorRojo, valorVerde, valorAzul);` asigna los valores de 0 a 255 para cada uno de los tres colores.
- `colorPredefinido(nombreDelColor);` acepta algunos Strings de colores, como `"blanco"` or `"amarillo"`. Si el color que introduces no está en nuestra lista, el LED se pondrá negro.
- `sumar(sumarRojo, sumarVerde, sumarAzul);` suma el valor dado al valor actual del LED (hasta llegar a 255).
- `restar(quitarRojo, quitarVerde, quitarAzul);` resta desde el valor al que está puesto el LED (hasta llegar a cero).
- `apagar();` pone todos los valores a cero.
- `testear();` hace una prueba de los tres colores, encendiendo cada uno 300ms y después todos a la vez (blanco) durante 500ms.
- `efectoFlash(tiempoOn, tiempoOff)` hace efecto de flash una vez con los parámetros dados en milisegundos.

## Contribuir
Si quieres contribuir a la librería añadiendo nuevas funciones o mejorando las actuales, no tienes más que crear PRs con todo lo que quieras ver en las nuevas versiones. Además, si encuentras errores o crees que hay cosas por mejorar, puedes crear un nuevo issue.

