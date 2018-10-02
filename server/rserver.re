open Lwt.Infix;

module List = Core.List;

/* In the future it should send at specified time every day instead of waiting for 24h */
let day_in_seconds = float_of_int(60 * 60 * 24);

let compose_mail = () =>
  Netsendmail.compose(
    ~from_addr=("Weather server", Config.emailUser),
    ~to_addrs=[("Weather subscriber", Config.emailRecipient)],
    ~subject="Drying clothes",
    "Hello, you should dry your clothes",
  );

let should_email =
  fun
  | None => false
  | Some((stats: Parser.stats)) => stats.rain_status == "None";

let send_email = client => Mail_client.send_mail_sf(compose_mail(), client);

let try_send = (client, analysis) => {
  if (should_email(analysis)) {
    send_email(client) |> ignore;
    print_endline("Email sent");
  } else {
    print_endline("Will not send email");
  };
  Lwt.return();
};

let rec main_loop = mail_client =>
  Client.request_forcast()
  >|= (n => n |> Forecast.analyse |> List.hd)
  >>= try_send(mail_client)
  >>= (() => Lwt_unix.sleep(day_in_seconds))
  >>= (() => main_loop(mail_client));

let init = () => {
  Nettls_gnutls.init() |> ignore;
  let mail_client =
    Mail_client.create("smtp.gmail.com", 587)
    |> Mail_client.authenticate_sf(Config.emailUser, Config.emailPass);
  main_loop(mail_client) |> Lwt_main.run;
};

let _ = init();