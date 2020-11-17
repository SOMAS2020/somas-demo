## What am I seeing?

A `C++` "server" capable of sending requests to a Python and a C++ "client", all three in different processes.

## Setting up

I haven't set up a Dockerfile. Just running on Ubuntu 20.04 on WSL2 for the time being.

- Set up [zmqpp](https://github.com/zeromq/zmqpp) (this is the largest obstacle, and hence is why we shouldn't _walk this way_)
- `python3 -m pip install pyzmq` (this should be in a `venv`, but I can't be bothered atm)
- `./run.sh`

## Structure

### `src`, `include`
Code for server

### `client`
Code for client--each folder is a client and currently I have C++ and Python3 demos.
The naming format is `client<LANGUAGEID>_<PORTNO>`. Each folder has `build.sh` and `run.sh` scripts to, well, do what they're named after. 

## My thoughts
- I initially thought 0MQ would open up cross platform capabilities--this is quite far from the truth... This setup would require everyone to do stuff on a Docker image, and to be honest, I'm definitely not that keen as this is SOMAS, not a course on actual distributed cross-platform software engineering (Paxos pls). The overhead in setting this up is quite high.
- In 2020 hindsight (there's only 2 months left to say this) I designed the server _very badly_, there's a lot of code for this MWE. Though, even if I designed it better, to attain the same functionality would still use a lot of code. Again, this is SOMAS!
- While 0MQ tries to abstract away a lot of raw sockets and IPC stuff, it'll still take some time to understand what's happening under the hood, which I'm sure will be required as it's going to need to be set up and run on 40 environments, even with Docker (I personally wasted hours building images on Docker in WSL2 just to realise that Windows' virtualisation is still far from perfect - [macOS isn't that much better either](https://docs.docker.com/docker-for-mac/networking/#known-limitations-use-cases-and-workarounds)).

## More thoughts
### REST endpoints
- Could do, but this still needs setup, and people would need to learn its models. I don't actually know how _portable_ code for this can be.

### Redefining our requirements
- Low overhead
- Easy to set-up
- An ecosystem friendly to most
- Cross-platform 
- Portable code
- Not pure Python or JS

### Just do it in one language and process?
- Yes. I actually think this is the easiest. 
- We know Prolog is a pain already, so let's cross that off.
- Let's be honest, Python 3 with `mypy` (don't worry if you don't know what this is _yet_, it makes Python much more "statically safer") is probably our best bet for everyone to be productive right off the bat.
- .NET is another alternative, giving people the choice of F# and C#. Personally I like these two languages and the ecosystem, however there _is_ a learning curve (C# isn't actually too hard if you did C++, and F# is easy for people who did HLP).

#### My thoughts on the above... thoughts
- Python 3 with `mypy`, in a tightly controlled `venv` handled by dedicated infra people.
- *Tightly* enforced *types* for APIs. This is better than the 0MQ/REST endpoints approach.
- Python has rather OK-ish `async` methods we can exploit, making the "distributed" system model we're trying to emulate not so painful.

#### My thoughts on how we can proceed with Python 3 with `mypy`

- Code structure 
    - The same submodule model (one server git repo with many island submodules)
    - The server island's `main` branch is restricted -- all changes must be PR-ed
    - PRs will have Github actions running the simulation (and tests if present), then posted as an issue/somewhere/a server

- Code design 
    - Server will have a Model for a client, this should be a class where the clients inherit it. With this class also brings tightly controlled `mypy` types for the interface

#### What about .NET?

- Mostly similar to the Python 3 with `mypy` approach, but types are _strong_ and _free_! (please again note learning curve...)

#### My most honest thoughts
- If we do IPC
    - GraphQL endpoints
- If we do one language
    - TypeScript or Golang (I miss my channel-`chan`), sorry.

##### If you're still reading
dayum. thanks for actually caring :heart_eyes: