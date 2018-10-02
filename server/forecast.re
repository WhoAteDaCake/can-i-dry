module List = Core.List;
module Option = Core.Option;

let notEqBy = (fn, a, b) => fn(a) != fn(b);

let no_time_date = (forecast: Parser.forecast) =>
  String.split_on_char(' ', forecast.date) |> List.hd_exn;

let get_day = (forecast: Parser.forecast) =>
  no_time_date(forecast)
  |> String.split_on_char('-', _)
  |> List.rev
  |> List.hd_exn;

let sum_fields = (fn, list) =>
  List.fold_left(list, ~init=0.0, ~f=(sum, entry) => sum +. fn(entry));

let calculate_rain =
  sum_fields((forecast: Parser.forecast) =>
    Option.value(forecast.rain_precipitation, ~default=0.0)
  );
let calculate_temp = sum_fields((forecast: Parser.forecast) => forecast.temp);

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

let group_by_day = List.group(_, ~break=notEqBy(get_day));

let get_stats = (day: list(Parser.forecast)) : Parser.stats => {
  let rain_mm = calculate_rain(day);
  {
    date: no_time_date(day |> List.hd_exn),
    temp: calculate_temp(day) /. float_of_int(List.length(day)),
    rain_status: Rain.intensity(rain_mm, max_h_rain(day)),
    rain_mm,
  };
};

let analyse = list => {
  let result = group_by_day(list);
  List.map(result, ~f=get_stats);
};