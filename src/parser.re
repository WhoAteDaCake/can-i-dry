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