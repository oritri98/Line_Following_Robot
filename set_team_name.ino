void team_name() {
  oled.clear();
  oled.set2X();
  oled.setInvertMode(0);
  text("MECHAFORGE", 4, 3);
}

void text(String t, byte x, byte y) {
  oled.setCursor(x, y);
  oled.print(t);
}