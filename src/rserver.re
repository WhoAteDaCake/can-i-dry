open Lwt.Infix;

/* open Core;

   module Server = Opium.Std;
   module Client = Redis_lwt.Client;
   module App = Server.App;

   let resp_to_text = opt =>
     switch (opt) {
     | Some(opt) => opt
     | None => "Not found"
     };

   let connection = Client.connect({host: "localhost", port: 7000});
   /* Unwraps connection */
   let connected = fn => connection >>= fn;

   let get_data =
     Server.get("/redis/:channel/:hash", req => {
       let channel = Server.param(req, "channel");
       let hash = Server.param(req, "hash");
       Client.hget(_, channel, hash)
       |> connected
       >|= resp_to_text
       >>= (text => Server.(`String(text) |> respond'));
     });

   let create_data =
     Server.post("/tasks/:channel", req => {
       let channel = Server.param(req, "channel");
       App.json_of_body_exn(req)
       >|= Parser.task_from_json
       >>= (task => Client.hset(_, channel, task.id, task.name) |> connected)
       >|= (_x => Server.(`String("Done") |> respond));
     });

   let _ = App.empty |> get_data |> create_data |> App.run_command; */