open System.IO
open System

let path = "./"
let out = "combine.csv"

Directory.GetFiles(path, "*.csv")
|> Array.where (fun x -> Path.GetFullPath out <> Path.GetFullPath x)
|> fun files ->
files
|> Array.map File.ReadAllLines
|> Array.transpose
|> Array.append [| files |> Array.map Path.GetFileNameWithoutExtension |]
|> Array.map (String.concat ",")
|> fun x -> File.WriteAllLines(out, x)