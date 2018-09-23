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

let get_or_default = (json, path, default) =>
  if (Ezjsonm.mem(json, path)) {
    Ezjsonm.find(json, path);
  } else {
    default;
  };

let get_opt = (json, path) =>
  if (Ezjsonm.mem(json, path)) {
    Some(Ezjsonm.find(json, path));
  } else {
    None;
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
    |> List.filter(_, ~f=((dict_key, _value)) => dict_key === key)
    |> List.hd
    |> opt => Option.(opt  >>| ((_key, value)) => fn(value))
  );

let get_temp = double_nested("main", "temp", Ezjsonm.get_float);
let get_rain = double_nested("rain", "3h", Ezjsonm.get_float);
/* let get_temp = json =>
  Ezjsonm.(
    find(json, ["main"])
    |> get_dict
    |> List.filter(_, ~f=((key, _value)) => key === "temp")
    |> List.hd
    |> opt => Option.(opt  >>| ((_key, value)) => get_float(value))
  );
let get_rain = json =>
  Ezjsonm.(
    find(json, ["rain"])
    |> get_dict
    |> List.filter(_, ~f=((key, _value)) => key === "3h")
    |> List.hd
    |> opt => Option.(opt  >>| ((_key, value)) => get_float(value))
  ); */

let forecast_from_json = json => {
  Ezjsonm.{
    time: find(json, ["dt"]) |> get_int |> to_milisecond,
    date: find(json, ["dt_txt"]) |> get_string,
    temp: json |> get_temp |> Option.value_exn(_),
    rain_precipitation: Some(20.2),
    /* rain_precipitation: json |> get_rain, */
    weather: {
      main: "test",
      description: "test"
    }
    /* weather:
      find(json, ["weather"]) |> get_list(weather_from_json) |> List.hd_exn, */
  };
};

let extract_forecasts = json =>
  Ezjsonm.(find(json, ["list"]) |> get_list(forecast_from_json));

/* let json_from_forecast = forecast => Ezjsonm.(dict); */

/* let format_forecasts = Ezjsonm.list(json_from_forecast); */
/* type task = {
     id: string,
     name: string,
   };

   let gen_uuid = () => Uuidm.v5(Uuidm.ns_dns, "www.rserver.com") |> Uuidm.to_string;

   let get_or = (json, path, default) =>
     if (Ezjsonm.mem(json, path)) {
       Ezjsonm.find(json, path);
     } else {
       default;
     };

   let task_from_json = json_raw => {
     let json = Ezjsonm.value(json_raw);
     Ezjsonm.({
       name: find(json, ["name"]) |> get_string,
       id: get_or(json, ["id"], "task-" ++ gen_uuid() |> string) |> get_string,
     });
   }; */