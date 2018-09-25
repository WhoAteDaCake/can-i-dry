/*
   All measurements are in mm
   http://wiki.sandaysoft.com/a/Rain_measurement
 */
let none = 0.0;
let very_light = 0.25;
let light = 1.0;
let moderate = 4.0;
let heavy = 16.0;
let very_heavy = 50.0;

let score_to_text = n =>
  switch (n) {
  | 0 => "None"
  | 1 => "Very light rain"
  | 2 => "Light rain"
  | 3 => "Moderate rain"
  | 4 => "Heavy rain"
  | 5 => "Very heavy rain"
  | _ => "Extreme rain"
  };
let calculate_intensity = (multiplier, mm) =>
  if (mm == 0.0) {
    0;
  } else if (mm <= very_light *. multiplier) {
    1;
  } else if (mm <= light *. multiplier) {
    2;
  } else if (mm <= moderate *. multiplier) {
    3;
  } else if (mm <= heavy *. multiplier) {
    4;
  } else if (mm <= very_heavy *. multiplier) {
    5;
  } else {
    3;
  };
let daily_intensity = calculate_intensity(24.0);
let hourly_intensity = calculate_intensity(1.0);
let intensity = (daily, hourly) =>
  max(daily_intensity(daily), hourly_intensity(hourly)) |> score_to_text;