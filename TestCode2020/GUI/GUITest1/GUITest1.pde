import controlP5.*;

ControlP5 cp5; //<>//
int v1;

void setup() {
  size(1920, 1080);  
  noStroke();
  cp5 = new ControlP5(this);
  cp5.addSlider("v1")
     .setPosition(40, 40)
     .setSize(200, 20)
     .setRange(100, 300)
     .setValue(250)
     .setColorCaptionLabel(color(20,20,20));
}

void draw() {
  background(200, 200, 200);

  pushMatrix();

  pushMatrix();
  fill(255, 255, 0);
  rect(v1, 100, 60, 200);
  fill(0, 255, 110);
  rect(40, v1, 320, 40);
  translate(200, 200);
  rotate(map(v1, 100, 300, -PI, PI));
  fill(255, 0, 128);
  rect(0, 0, 100, 100);
  popMatrix();

  translate(600, 100);
  for (int i=0; i<20; i++) {
    pushMatrix();
    fill(55);
    translate(0, i*10);
    rotate(map(v1+i, 0, 300, -PI, PI));
    rect(-150, 0, 300, 4);
    popMatrix();
  }

  popMatrix();
}
