open Lwt;
open Cohttp_lwt_unix;

let build_url = () =>
  Config.(
    baseUrl ++ "?id=" ++ cityId ++ "&units=" ++ units ++ "&APPID=" ++ appId
  )
  |> Uri.of_string;

let body =
  build_url()
  |> Client.get
  >>= (((_resp, body)) => Cohttp_lwt.Body.to_string(body))
  >|= (
    str =>
      str |> Ezjsonm.from_string |> Ezjsonm.value |> Parser.extract_forecasts
  );

let () =
  Lwt_main.run(
    body,
    /* let resp = Lwt_main.run(body);
       print_endline("Received body\n" ++ resp); */
  )
  |> ignore;