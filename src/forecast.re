/* https://en.wikipedia.org/wiki/Rain#Intensity */

module List = Core.List;
module Option = Core.Option;

module Rain = {
  /* All measurements are in mm */
  module Hourly = {
    let light = 2.5;
    let moderate = 7.6;
    let heavy = 50.0;
    let violent = 100.0;
  };
  module Daily = {
    let light = 2.5 *. 24.0;
    let moderate = 7.6 *. 24.0;
    let heavy = 50.0 *. 24.0;
    let violent = 100.0 *. 24.0;
  };
  let score_to_text = n =>
    switch (n) {
    | 0 => "Light"
    | 1 => "Moderate"
    | 2 => "Heavy"
    | _ => "Violent"
    };
  let hourlyIntensity = mm =>
    Hourly.(
      if (mm < light) {
        0;
      } else if (mm < moderate) {
        1;
      } else if (mm < heavy) {
        2;
      } else {
        3;
      }
    );
  let dailyIntensity = mm =>
    Daily.(
      if (mm < light) {
        0;
      } else if (mm < moderate) {
        1;
      } else if (mm < heavy) {
        2;
      } else {
        3;
      }
    );
  let intensity = (daily, hourly) =>
    max(dailyIntensity(daily), hourlyIntensity(hourly)) |> score_to_text;
};

let notEqBy = (fn, a, b) => fn(a) != fn(b);

let no_time_date = (forecast: Parser.forecast) =>
  String.split_on_char(' ', forecast.date) |> List.hd_exn;

let get_day = (forecast: Parser.forecast) =>
  no_time_date(forecast)
  |> String.split_on_char('-', _)
  |> List.rev
  |> List.hd_exn;

let calculate_rain = day =>
  List.fold_left(day, ~init=0.0, ~f=(sum, forecast: Parser.forecast) =>
    sum +. Option.value(forecast.rain_precipitation, ~default=0.0)
  );
let max_h_rain = day =>
  List.fold_left(
    day,
    ~init=0.0,
    ~f=(max, forecast: Parser.forecast) => {
      let rain = Option.value(forecast.rain_precipitation, ~default=0.0);
      if (rain > max) {
        rain;
      } else {
        max;
      };
    },
  )
  /* Need this because weather report returns 3h period */
  |> (n => n /. 3.0);

/* let hourly_rain_intensity = mm => if (mm < 2.5) */

let group_by_day = List.group(_, ~break=notEqBy(get_day));

let get_stats = (day: list(Parser.forecast)) : Parser.stats => {
  let rain_mm = calculate_rain(day);
  {
    date: no_time_date(day |> List.hd_exn),
    temp: 0.0,
    rain_status: Rain.intensity(rain_mm, max_h_rain(day)),
    rain_mm,
  };
};

let analyse = list => {
  let result = group_by_day(list);
  let _stats = List.map(result, ~f=get_stats);
  result;
};