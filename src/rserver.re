open Lwt.Infix;

module Server = Opium.Std;
module App = Server.App;

let get_forecast =
  Server.get("/forecast", _req =>
    Client.request_forcast()
    >|= (
      forecast =>
        forecast
        |> (
          fs =>
            {
              let list = Utils.group_by_day(fs);
              list
            }
            |> Parser.format_grouped_forecasts
            |> (json => `Json(json) |> Server.respond)
        )
    )
  );

let _ = App.empty |> get_forecast |> App.run_command;