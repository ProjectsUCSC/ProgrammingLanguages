/**
  * Created by shubhi on 6/11/17.
  */
class Parser(fpath: String) {
  def parse(regex: String, test_str: String): Unit = {
    import scala.sys.process._
    var contents = ""

    try {
      contents = (fpath + regex + " " + test_str).!!
    } catch {
      case e: Exception =>
        var exit_code = (fpath + regex + " " + test_str).!;
        if (exit_code == 1) println(" no match found !")
        else
          print(" non 0 exit code, please examine your query " + exit_code)
    }
    println(contents)
  }

  def interactive_parser(): Unit = {

    val regex = readLine("Please enter the regex ")
    val test_str = readLine("Please enter the test string ")
    parse(regex, test_str)
    var answer = readLine("Do you want to continue? y/N ")
    if (answer == "y") interactive_parser()

  }

}

object Parser {

  def main(args: Array[String]): Unit = {
    val fpath = "/Users/shubhi/Public/ProgrammingLanguages/parser "
    val p = new Parser(fpath)
    p.interactive_parser()
  }

}