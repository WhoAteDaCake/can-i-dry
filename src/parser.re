module Option = Core.Option;
module List = Core.List;

type weather = {
  main: string,
  description: string,
};

type forecast = {
  time: int,
  date: string,
  temp: float,
  rain_precipitation: option(float),
  weather,
};

let to_milisecond = n => n * 1000;

let weather_from_json = json =>
  Ezjsonm.{
    main: find(json, ["main"]) |> get_string,
    description: find(json, ["description"]) |> get_string,
  };

let double_nested = (path, key, fn, json) => 
  Ezjsonm.(
    find(json, [path])
    |> get_dict
    |> List.filter(_, ~f=((dict_key, _value)) => dict_key == key)
    |> List.hd
    |> opt => Option.(opt  >>| ((_key, value)) => fn(value))
  );

let get_temp = double_nested("main", "temp", Ezjsonm.get_float);
let get_rain = double_nested("rain", "3h", Ezjsonm.get_float);

let forecast_from_json = json => {
  Ezjsonm.{
    time: find(json, ["dt"]) |> get_int |> to_milisecond,
    date: find(json, ["dt_txt"]) |> get_string,
    temp: json |> get_temp |> Option.value_exn(_),
    rain_precipitation: json |> get_rain,
    weather:
      find(json, ["weather"]) |> get_list(weather_from_json) |> List.hd_exn,
  };
};

let extract_forecasts = json =>
  Ezjsonm.(find(json, ["list"]) |> get_list(forecast_from_json));