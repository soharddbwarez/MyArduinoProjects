 
// Speed for wait times.
int wait = 150;
 
// Naming and declaring the pins for the LED cube.
int topLayer = 11;
int midLayer = 12;
int botLayer = 13;

int col1 = 2;
int col2 = 3;
int col3 = 4;
int col4 = 5;
int col5 = 6;
int col6 = 7;
int col7 = 8;
int col8 = 9;
int col9 = 10;

void setup() {                
  // Setting up the pins.
  pinMode(topLayer, OUTPUT); 
  pinMode(midLayer, OUTPUT);
  pinMode(botLayer, OUTPUT);
  
  pinMode(col1, OUTPUT);
  pinMode(col2, OUTPUT);
  pinMode(col3, OUTPUT);
  pinMode(col4, OUTPUT);
  pinMode(col5, OUTPUT);
  pinMode(col6, OUTPUT);
  pinMode(col7, OUTPUT);
  pinMode(col8, OUTPUT);
  pinMode(col9, OUTPUT);
}

void loop() {
  
  // Main Program for the cube.
  
  // **************************
  // FACES FORWARD
  // **************************
  
  // Rear Face on.
  digitalWrite(col1, HIGH);
  digitalWrite(col2, HIGH);
  digitalWrite(col3, HIGH);
  
  //Turns all LED layers on.
  digitalWrite(botLayer, LOW);
  delay(wait);
  digitalWrite(midLayer, LOW);
  delay(wait);
  digitalWrite(topLayer, LOW);
  delay(wait);
  
  // Rear Face off.
  digitalWrite(col1, LOW);
  digitalWrite(col2, LOW);
  digitalWrite(col3, LOW);
  
  // Middle Face.
  digitalWrite(col4, HIGH);
  digitalWrite(col5, HIGH);
  digitalWrite(col6, HIGH);
  delay(wait);
  digitalWrite(col4, LOW);
  digitalWrite(col5, LOW);
  digitalWrite(col6, LOW);

  // Front Face on.
  digitalWrite(col7, HIGH);
  digitalWrite(col8, HIGH);
  digitalWrite(col9, HIGH);
  delay(wait);
  
  // Turns all LED layers off.
  digitalWrite(topLayer, HIGH);
  delay(wait);
  digitalWrite(midLayer, HIGH);
  delay(wait);
  digitalWrite(botLayer, HIGH);
  delay(wait);
  
  // Front Face off.
  digitalWrite(col7, LOW);
  digitalWrite(col8, LOW);
  digitalWrite(col9, LOW);
  
  // **************************
  // LAYERS
  // **************************
  
  //Turns all LED columns on.
  digitalWrite(col1, HIGH);
  digitalWrite(col2, HIGH);
  digitalWrite(col3, HIGH);
  digitalWrite(col4, HIGH);
  digitalWrite(col5, HIGH);
  digitalWrite(col6, HIGH);
  digitalWrite(col7, HIGH);
  digitalWrite(col8, HIGH);
  digitalWrite(col9, HIGH);
  
  // Bottom layer.
  digitalWrite(botLayer, LOW);
  delay(wait);
  digitalWrite(botLayer, HIGH);
  
  // Middle layer.
  digitalWrite(midLayer, LOW);
  delay(wait);
  digitalWrite(midLayer, HIGH);

  // Top layer.
  digitalWrite(topLayer, LOW);
  delay(wait);
  digitalWrite(topLayer, HIGH);
  
  // Middle layer.
  digitalWrite(midLayer, LOW);
  delay(wait);
  digitalWrite(midLayer, HIGH);
  
  // Bottom layer.
  digitalWrite(botLayer, LOW);
  delay(wait);
  digitalWrite(botLayer, HIGH);
  
  //Turns all LED columns off.
  digitalWrite(col1, LOW);
  digitalWrite(col2, LOW);
  digitalWrite(col3, LOW);
  digitalWrite(col4, LOW);
  digitalWrite(col5, LOW);
  digitalWrite(col6, LOW);
  digitalWrite(col7, LOW);
  digitalWrite(col8, LOW);
  digitalWrite(col9, LOW);
  
  // **************************
  // FACES BACK
  // **************************
  
  // Front Face on.
  digitalWrite(col7, HIGH);
  digitalWrite(col8, HIGH);
  digitalWrite(col9, HIGH);
  
  //Turns all LED layers on.
  digitalWrite(botLayer, LOW);
  delay(wait);
  digitalWrite(midLayer, LOW);
  delay(wait);
  digitalWrite(topLayer, LOW);
  delay(wait);
  
  // Front Face off.
  digitalWrite(col7, LOW);
  digitalWrite(col8, LOW);
  digitalWrite(col9, LOW);
  
  // Middle Face.
  digitalWrite(col4, HIGH);
  digitalWrite(col5, HIGH);
  digitalWrite(col6, HIGH);
  delay(wait);
  digitalWrite(col4, LOW);
  digitalWrite(col5, LOW);
  digitalWrite(col6, LOW);
  
    // Rear Face on.
  digitalWrite(col1, HIGH);
  digitalWrite(col2, HIGH);
  digitalWrite(col3, HIGH);
  delay(wait);
  
  // Turns all LED layers off.
  digitalWrite(topLayer, HIGH);
  delay(wait);
  digitalWrite(midLayer, HIGH);
  delay(wait);
  digitalWrite(botLayer, HIGH);
  delay(wait);
  
   // Rear Face off.
  digitalWrite(col1, LOW);
  digitalWrite(col2, LOW);
  digitalWrite(col3, LOW);

  
  // **************************
  // LAYERS
  // **************************
  
  //Turns all LED columns on.
  digitalWrite(col1, HIGH);
  digitalWrite(col2, HIGH);
  digitalWrite(col3, HIGH);
  digitalWrite(col4, HIGH);
  digitalWrite(col5, HIGH);
  digitalWrite(col6, HIGH);
  digitalWrite(col7, HIGH);
  digitalWrite(col8, HIGH);
  digitalWrite(col9, HIGH);
  
  // Bottom layer.
  digitalWrite(botLayer, LOW);
  delay(wait);
  digitalWrite(botLayer, HIGH);
  
  // Middle layer.
  digitalWrite(midLayer, LOW);
  delay(wait);
  digitalWrite(midLayer, HIGH);

  // Top layer.
  digitalWrite(topLayer, LOW);
  delay(wait);
  digitalWrite(topLayer, HIGH);
  
  // Middle layer.
  digitalWrite(midLayer, LOW);
  delay(wait);
  digitalWrite(midLayer, HIGH);
  
  // Bottom layer.
  digitalWrite(botLayer, LOW);
  delay(wait);
  digitalWrite(botLayer, HIGH);
  
  //Turns all LED columns off.
  digitalWrite(col1, LOW);
  digitalWrite(col2, LOW);
  digitalWrite(col3, LOW);
  digitalWrite(col4, LOW);
  digitalWrite(col5, LOW);
  digitalWrite(col6, LOW);
  digitalWrite(col7, LOW);
  digitalWrite(col8, LOW);
  digitalWrite(col9, LOW);
}
