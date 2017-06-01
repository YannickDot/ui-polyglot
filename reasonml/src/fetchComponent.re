module FetchComponent = {
  include ReactRe.Component.Stateful.InstanceVars;
  let name = "FetchComponent";
  type props = { url: string };
  type state = { json: option string };
  type instanceVars = {mutable request: option Js_global.timeoutId};

  let getInstanceVars () => {request: None};
  let getInitialState _ => {json: None};

  let updateJson {state} => {
    let newJson = switch state.json {
      | None => Some "req"
      | Some json => Some (json ^ " > req")
    };
    {json: newJson};
  };

  let startReq {instanceVars, setState} _ => {
    instanceVars.request =
    Some
      (Js_global.setTimeout
        (fun () => {
          setState updateJson;
        })
        1000);
    None;
  };

  let styling = ReactDOMRe.Style.make
    padding::"30px"
    color::"white"
    backgroundColor::"rebeccapurple" ();

  let btnStyle = ReactDOMRe.Style.make
    padding::"10px 15px"
    backgroundColor::"green"
    color::"white" ();

  let renderJson state => {
    switch state.json {
      | None => ""
      | Some json => json
    };
  };

  let render {state, updater, props} =>
    <div style=(styling)>
      <div>(ReactRe.stringToElement "I'm a network fetching component !")</div>
      <button onClick=(updater startReq)>
        (ReactRe.stringToElement "start")
      </button>
      <div>
        <p>
          (ReactRe.stringToElement ("url : " ^ props.url))
        </p>
        <p>
          (ReactRe.stringToElement "json : ")
          (ReactRe.stringToElement (renderJson state))
        </p>
      </div>
    </div>;
};

include ReactRe.CreateComponent FetchComponent;

let createElement ::url => wrapProps {url: url};
