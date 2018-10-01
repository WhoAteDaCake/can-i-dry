open Lwt.Infix;

/* let rec echo_server = () =>
   Lwt_io.printl("Message")
   >>= (() => Lwt_io.flush(Lwt_io.stdout))
   >>= (() => Lwt_unix.sleep(1.0))
   >>= (() => echo_server()); */
/* let rec echo_server =
   fun
   | 0 => Lwt.return()
   | n => {
       print_endline("Before");
       Lwt_io.printl("Message")
       >>= (() => Lwt_io.flush(Lwt_io.stdout))
       >>= (() => sleep(1.0))
       >>= (() => echo_server(n - 1));
     }; */

let init = () => {
  Nettls_gnutls.init() |> ignore;
  let mail_client =
    Mail_client.create("smtp.gmail.com", 587)
    |> Mail_client.authenticate_sf(Config.emailUser, Config.emailPass);
  ();
};

let _ = init();