/* Has side-effects */
module Option = Core.Option;
let enable_debug_sf = () => Netsmtp.Debug.enable := true;

type mail_client = {
  domain: string,
  client: Netsmtp.client,
  tls: (module Netsys_crypto_types.TLS_CONFIG),
};

let default_timeout = 60.0;

let create = (~timeout=?, domain, port) => {
  let timeout = Option.value(timeout, ~default=default_timeout);
  let provider = Netsys_crypto.current_tls();

  let address =
    `Socket((
      `Sock_inet_byname((Unix.SOCK_STREAM, domain, port)),
      Uq_client.default_connect_options,
    ));

  let tls =
    Netsys_tls.create_x509_config(
      ~peer_auth=`None,
      ~system_trust=true,
      provider,
    );
  let client = (new Netsmtp.connect)(address, timeout);
  {domain, client, tls};
};

let authenticate_sf = (user, password, mail_client) => {
  mail_client.client#helo() |> ignore;
  mail_client.client#starttls(
    mail_client.tls,
    ~peer_name=Some(mail_client.domain),
  );

  mail_client.client#auth(
    (module Netmech_plain_sasl.PLAIN),
    user,
    "",
    [("password", password, [])],
    [],
  );
  mail_client;
};

let send_mail_sf = (mail, mail_client) => {
  Netsmtp.sendmail(mail_client.client, mail);
  mail_client;
};