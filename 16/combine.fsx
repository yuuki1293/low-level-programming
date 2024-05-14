open System.IO
open System

let path = "./"
let out = "combine.csv"

let files = Directory.GetFiles(path, "*.csv") |> Array.where (fun x -> Path.GetFullPath out <> Path.GetFullPath x)
let data = [ for fName in files do yield File.ReadAllLines fName |> Array.toList ]

seq {
    yield files |> Array.toSeq |> Seq.map Path.GetFileNameWithoutExtension
    for i in 0..data[0].Length-1 do
        yield seq {
            for d in data do
                yield d[i]
        }
}
|> Seq.map (fun x -> String.Join(",", x))
|> Seq.toList
|> fun x -> File.WriteAllLines(out, x)