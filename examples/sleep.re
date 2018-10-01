open Lwt.Infix;

let sleep = s => Lwt_unix.sleep(s);

let rec echo_server =
  fun
  | 0 => Lwt.return()
  | n =>
    Lwt_io.printl("Message")
    >>= (() => Lwt_io.flush(Lwt_io.stdout))
    >>= (() => sleep(1.0))
    >>= (() => echo_server(n - 1));

let _ = echo_server(10) |> Lwt_main.run;