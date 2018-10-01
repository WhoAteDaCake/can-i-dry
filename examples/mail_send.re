Nettls_gnutls.init() |> ignore;

let mail =
  Netsendmail.compose(
    ~from_addr=("Test user", Config.emailUser),
    ~to_addrs=[("Recipient", Config.emailRecipient)],
    ~subject="Test subject2",
    "Hello, this is a test email 2",
  );

let _ =
  Mail_client.create("smtp.gmail.com", 587)
  |> Mail_client.authenticate_sf(Config.emailUser, Config.emailPass)
  |> Mail_client.send_mail_sf(mail);