module List = Core.List;

let get_day = (forecast: Parser.forecast) =>
  String.split_on_char(' ', forecast.date)
  |> List.hd_exn
  |> String.split_on_char('-', _)
  |> List.rev
  |> List.hd_exn;

let group_by_day = List.group(_, ~break=(a, b) => {
  get_day(a) != get_day(b)
});