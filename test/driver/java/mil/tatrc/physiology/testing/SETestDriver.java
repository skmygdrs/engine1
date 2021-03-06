/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package mil.tatrc.physiology.testing;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.*;

import com.kitware.physiology.cdm.Enums.eSwitch;

import mil.tatrc.physiology.datamodel.engine.SEAutoSerialization;
import mil.tatrc.physiology.datamodel.properties.CommonUnits.TimeUnit;
import mil.tatrc.physiology.utilities.FileUtils;
import mil.tatrc.physiology.utilities.Log;
import mil.tatrc.physiology.utilities.LogListener;
import mil.tatrc.physiology.utilities.RunConfiguration;
import mil.tatrc.physiology.utilities.UnitConverter;
import mil.tatrc.physiology.testing.csv.CSVComparison;
import mil.tatrc.physiology.utilities.csv.plots.CSVComparePlotter;
import mil.tatrc.physiology.utilities.csv.plots.CSVComparePlotter.PlotType;

public class SETestDriver
{
  static
  {        
    UnitConverter.initialize(System.getProperty("user.dir"));
  }

  public interface Executor
  {
    boolean ExecuteTest(TestJob job);
  }

  public static void main(String[] args)
  {
    SETestDriver me = new SETestDriver();
    RunConfiguration cfg = new RunConfiguration();
    
    if(args.length == 0)
    {
      Log.error("No configFile specified");
      return;
    }
    if(args.length == 4)
    {
      me.commitHash = args[1];
      me.environment = args[2];
      me.architecture = args[3];
    }
    File configFile = new File(cfg.getTestConfigDirectory()+"/"+args[0]);
    if(!configFile.exists())
    {
      System.err.println("Config file "+configFile.getName()+" does not exist");
      return;
    }
    me.processConfigFile(configFile, cfg.getVerificationDirectory());

    int     availableThreads = Runtime.getRuntime().availableProcessors();
    boolean isPlotting;
    int     activeThreads=0;
    int     engineThreads=1;
    if(me.numThreads <= 0)
      engineThreads = availableThreads + me.numThreads;
    else
      engineThreads = me.numThreads;
    if(engineThreads<=0)
      engineThreads = 1;
    if(engineThreads > availableThreads)
    {
      engineThreads = availableThreads;
      Log.info("You dont have " + me.numThreads + " available on your computer.");
    }

    Log.info("I can run " + availableThreads + " threads, I am going to use " + engineThreads);

    do
    {
      activeThreads = 0;
      isPlotting = false;
      for(TestJob job : me.jobs)
      {
        if(job.state == JobState.Executing)
          activeThreads++;
        if(job.state == JobState.Comparing)
        {
          activeThreads++;
          isPlotting = true;
        }
      }
      if(!isPlotting && activeThreads != engineThreads)
      {// Look for a job to compare first    
        for(TestJob job : me.jobs)
        {                      
          if(job.state == JobState.Executed)
          {
            if(job.PlottableResults && !job.skipPlot)
            {
              CompareThread cThread = me.new CompareThread();
              job.state = JobState.Comparing;
              cThread.job = job;
              activeThreads++;
              cThread.start();              
              break;
            }
            else
            {
              job.state = JobState.Complete;
            }            
          }
        }
      }
      if(activeThreads != engineThreads)
      {
        for(TestJob job : me.jobs)
        {
          if(job.state==JobState.Provisioned)
          {
            ExecuteThread eThread = me.new ExecuteThread();
            eThread.job = job;
            job.state = JobState.Executing;
            eThread.start();
            activeThreads++;
          }
          if(activeThreads == engineThreads)
            break;            
        }
      }      
      if(activeThreads==0)
        break;

      try
      {Thread.sleep(100);}
      catch (InterruptedException ex)
      { Log.error("I have insomnia...",ex); }
    }while(true);


    me.createReport();
  }
  
  public String environment = "";
  public String architecture = "";
  public String commitHash = "";

  protected String testName;
  protected String reportName;

  protected int    numThreads=0;
  protected double percentDifference=2.0;

  protected boolean useStates=false;
  protected SEAutoSerialization autoSerialization=null;
  protected String patientFiles;

  protected Map<String,String> macros = new HashMap<String,String>();
  protected Map<String,Class<SETestDriver.Executor>> executors = new HashMap<String,Class<SETestDriver.Executor>>();
  protected List<TestJob> jobs = new ArrayList<TestJob>();  

  protected Map<TestJob,String> job2groups = new HashMap<TestJob,String>();
  protected Map<String,List<String>> groups = new LinkedHashMap<String,List<String>>();

  protected enum JobState {Provisioned, Executing, Executed, Comparing, Complete}

  public class TestJob extends LogListener
  {
    public TestJob(){ listen(false);  }
    public boolean      useState=false;
    public SEAutoSerialization autoSerialization=null;
    public String       patientFile = null;
    public JobState     state=JobState.Provisioned;
    public String       name;
    public boolean      skipExecution = false;
    public boolean      skipPlot = false;
    public boolean      knownFailure = false;
    public SETestDriver.Executor executor = null;
    public boolean      PlottableResults = false;
    public PlotType     plotType = PlotType.FastPlot;//Only plot every nth data point
    public double       percentDifference;
    public List<String> baselineFiles = new ArrayList<String>();
    public String       baselineDirectory = null;
    public List<String> computedFiles = new ArrayList<String>();
    public String       computedDirectory = null;
    public List<String> reportFiles = new ArrayList<String>();
    public String       resultsFiles = null;

    public TestJob clone()
    {
      TestJob copy = new TestJob();
      copy.useState = this.useState;
      copy.patientFile = this.patientFile;
      copy.state = this.state;
      copy.name = this.name;
      copy.skipExecution = this.skipExecution;
      copy.skipPlot = this.skipPlot;
      copy.knownFailure = this.knownFailure;
      copy.executor = this.executor;
      copy.PlottableResults = this.PlottableResults;
      copy.plotType = this.plotType;
      copy.percentDifference = this.percentDifference;
      copy.baselineFiles.addAll(this.baselineFiles);
      copy.baselineDirectory = this.baselineDirectory;
      copy.computedFiles.addAll(this.computedFiles);
      copy.computedDirectory = this.computedDirectory;
      copy.reportFiles.addAll(this.reportFiles);
      copy.resultsFiles = this.resultsFiles;
      return copy;
    }

    public void handleDebug(String msg) { Log.debug("["+name+"] "+msg,""); }
    public void handleInfo(String msg)  { Log.info("["+name+"] "+msg,""); }
    public void handleWarn(String msg)  { Log.warn("["+name+"] "+msg,""); }
    public void handleError(String msg) { Log.error("["+name+"] "+msg,""); }
    public void handleFatal(String msg) { Log.fatal("["+name+"] "+msg,""); }
  }

  public void processConfigFile(File configFile, String baselineDir)
  {    
    
    
    this.testName = configFile.getName();
    this.testName = this.testName.substring(0,this.testName.lastIndexOf('.'));
    Log.setFileName("./test_results/"+this.testName+".log");

    String key;
    String value;

    this.percentDifference = 2.0;

    this.reportName = "TestDriver Report";

    this.executors.clear();
    this.jobs.clear();

    boolean ExecuteJobs=true;
    boolean PlotResults=true;
    String currentGroup = this.testName;

    // Parse the config file
    // TODO, make sure all \ are converted to /
    try
    {
      BufferedReader br = new BufferedReader(new FileReader(configFile));
      String line;
      while ((line = br.readLine()) != null)
      {
        if (line.length() == 0 || line.startsWith("#"))
          continue;
        if(line.startsWith("@group"))
        {
          currentGroup = line.substring(6).trim();
          if(currentGroup.isEmpty())
            currentGroup = this.testName;
          continue;
        }
        if(line.indexOf('=') == -1)
          continue;

        line = line.trim();
        key = line.substring(0, line.indexOf('='));
        value = line.substring(line.indexOf("=") + 1);

        if(key.equalsIgnoreCase("ReportName"))
        { this.reportName = value; continue; }

        if(key.equalsIgnoreCase("PercentDifference"))
        { this.percentDifference = Double.parseDouble(value); continue; }
        if(key.equalsIgnoreCase("Threads"))
        { this.numThreads = Integer.parseInt(value); continue; }
        if(key.equalsIgnoreCase("Patients"))
        { this.patientFiles = value; continue; }
        if(key.equalsIgnoreCase("UseStates"))
        {
          if(value.equalsIgnoreCase("true"))
            this.useStates=true; 
          continue; 
        }
        if(key.equalsIgnoreCase("AutoSerialization"))
        {
          this.autoSerialization=new SEAutoSerialization();
          // Parse the value
          String[] values = value.split(",");
          this.autoSerialization.setDirectory(values[0]);
          this.autoSerialization.getPeriod().setValue(Double.parseDouble(values[1]),TimeUnit.s);
          this.autoSerialization.setPeriodTimeStamps(eSwitch.valueOf(values[2]));    
          this.autoSerialization.setAfterActions(eSwitch.valueOf(values[3]));    
          this.autoSerialization.setReloadState(eSwitch.valueOf(values[4]));
          continue; 
        }
        if(key.equalsIgnoreCase("ExecuteTests"))
        {
          if(value.equalsIgnoreCase("false"))
            ExecuteJobs=false; 
          continue; 
        }
        if(key.equalsIgnoreCase("PlotResults"))
        {
          if(value.equalsIgnoreCase("false"))
            PlotResults=false; 
          continue; 
        }       
        if(key.equalsIgnoreCase("Executor"))
        {
          Class<SETestDriver.Executor> clazz = null;
          try
          {
            clazz = (Class<SETestDriver.Executor>)Class.forName(value);             
            this.executors.put(clazz.getSimpleName(), clazz);
          } 
          catch(Exception e){Log.error("Could not find Executor "+value);}
          continue;
        }        

        if(key.startsWith("Macro"))
        {
          macros.put(key.substring(6), value);
          continue;
        }

        for(String macro : macros.keySet())
        {
        	String buffer = " "+value+" ";
          if(buffer.indexOf(" "+macro+" ")>-1)
            value = buffer.replaceAll(" "+macro+" ", " "+macros.get(macro)+" ");
        }

        TestJob job = new TestJob();
        job.useState = this.useStates;
        job.autoSerialization = this.autoSerialization;
        if(!ExecuteJobs)
          job.skipExecution = true;
        if(!PlotResults)
          job.skipPlot = true;
        job2groups.put(job, currentGroup);
        if (key.charAt(0) == '*')
        {
          job.knownFailure = true;
          job.state = JobState.Complete;
          key = key.substring(1);
        }
        if (key.charAt(0) == '-')
        {
          job.skipExecution = true;
          job.skipPlot = true;
          key = key.substring(1);
        }
        this.jobs.add(job);
        job.name = key.trim();          
        job.percentDifference = this.percentDifference;

        String[] directives = value.trim().split(" ");
        for(String directive : directives)
        {
          if(directive.indexOf("=")==-1)
          {
            if(executors.containsKey(directive))
            {
              try{job.executor = executors.get(directive).newInstance();}
              catch (Exception e)
              {
                job.knownFailure = true;
                Log.error("Could not make a new "+executors.get(directive).getName()+" executor");                
              }
              continue;
            }
            if(directive.equalsIgnoreCase("NoCompare")) 
            { job.PlottableResults = false; continue; }
            if(directive.equalsIgnoreCase("FastPlot")) 
            { job.PlottableResults = true; job.plotType=PlotType.FastPlot; continue; }
            if(directive.equalsIgnoreCase("FullPlot"))
            { job.PlottableResults = true; job.plotType=PlotType.FullPlot; continue; }
            if(directive.equalsIgnoreCase("FullPlotErrors"))
            { job.PlottableResults = true; job.plotType=PlotType.FullPlotErrors; continue; }
            if(directive.equalsIgnoreCase("FastPlotErrors"))
            { job.PlottableResults = true; job.plotType=PlotType.FastPlotErrors; continue; }
            if(directive.equalsIgnoreCase("MemoryFastPlot"))
            { job.PlottableResults = true; job.plotType=PlotType.MemoryFastPlot; continue; }
          }
          else
          {
            key = directive.substring(0, directive.indexOf('='));
            value = directive.substring(directive.indexOf("=") + 1);
            if(key.equalsIgnoreCase("Baseline"))
            {job.baselineDirectory = baselineDir+"/"+value; continue;}
            if(key.equalsIgnoreCase("Scenario"))
            {job.baselineDirectory = value; continue;}
            else if(key.equalsIgnoreCase("Computed"))
            {job.computedDirectory = value; continue;}
            if(key.equalsIgnoreCase("Results"))
            {job.resultsFiles = value; continue;}   
          }
        }
        if(job.resultsFiles!=null && !job.resultsFiles.isEmpty())
        {
          for(String result : job.resultsFiles.split(","))
          {
            job.baselineFiles.add(job.baselineDirectory+"/"+result+".txt");
            job.computedFiles.add(job.computedDirectory+"/"+result+".txt");
          }
        }
        else
        {
          if(job.executor.getClass().getName().indexOf("Scenario")!=-1)//Is a Scenario File test
          {
            DeriveScenarioResultNames(job, job.name);            
          }
          else
          {
            job.baselineFiles.add(job.baselineDirectory+"/"+job.name+".txt");
            job.computedFiles.add(job.computedDirectory+"/"+job.name+".txt");
          }
        }
      }

      if(this.patientFiles!=null)
      {
        // Need to copy all the jobs and speficy a particular patient file
        List<String> patientFileNames;

        if(patientFiles.equalsIgnoreCase("all"))
          patientFileNames = FileUtils.findFiles("./patients", ".pba", true);        
        else
        {
          String[] patientFiles = this.patientFiles.split(",");
          patientFileNames = new ArrayList<String>();
          for(String patientFile : patientFiles)
          {
            patientFile = patientFile.trim();
            if(!patientFile.endsWith(".pba"))
              patientFile += ".pba";
            patientFileNames.add(patientFile);
          }
        }

        List<TestJob> oldJobs = new ArrayList<TestJob>(jobs);
        jobs.clear();
        TestJob copy;

        for(String pFileName : patientFileNames)
        {
          String[] split = pFileName.split("[/\\\\]");
          pFileName = split[split.length-1];

          for(TestJob job : oldJobs)
          {
            if(job.executor.getClass().getName().indexOf("Scenario")==-1)
            {
              jobs.add(job);
              continue;
            }
            copy = job.clone();                              
            copy.patientFile = pFileName;
            DeriveScenarioResultNames(copy, copy.name.replaceAll(".pba", "-"+pFileName));  
            jobs.add(copy);
          }
        }

      }

      // Let's clean out everything we are about to run
      for(TestJob job : jobs)
      {
        DeleteTestResults(job.computedDirectory+"/"+job.name,ExecuteJobs,PlotResults);
        for(String resultFile : job.computedFiles)
        {
          DeleteTestResults(resultFile,ExecuteJobs,PlotResults);
        }
      }
      br.close();
    }
    catch (IOException e)
    {
      Log.error("Ouch",e);
    }
  }

  protected void DeriveScenarioResultNames(TestJob job, String baseName)
  {
    job.baselineFiles.clear();
    job.computedFiles.clear();

    String[] dirs = baseName.substring(0, baseName.indexOf(".pba")).split("[/\\\\]");
    String baseline = job.baselineDirectory;
    for(int i=0; i<dirs.length-1; i++)
      baseline+="/"+dirs[i];
    baseline+="/"+dirs[dirs.length-1]+"Results.txt";
    job.baselineFiles.add(baseline);
    String output = job.computedDirectory;
    for(int i=0; i<dirs.length; i++)
      output+="/"+dirs[i];
    output+="Results.txt";
    //example : ./Scenarios/Validation/Patient-ValidationResults.txt
    job.computedFiles.add(output);
  }

  public class ExecuteThread extends Thread
  {
    public TestJob job;
    public void run()
    {          
      for(String toCompute : job.computedFiles)
      {
        if(job.PlottableResults)
        {
          if(job.name.endsWith(".pba"))//This should be a scenario file, different naming convention
          {
            String[] dirs = toCompute.substring(0,toCompute.indexOf(".txt")).split("[/\\\\]");
            String report = toCompute.substring(0,toCompute.indexOf(".txt"))+"/"+dirs[dirs.length-1]+"Report.pba";
            job.reportFiles.add(report);
          }
          else
          {
            String path = toCompute.substring(0, toCompute.lastIndexOf("."));
            path = path + path.substring(path.lastIndexOf("/")) + "Report.pba";
            job.reportFiles.add(path);
          }
        }
        else
          job.reportFiles.add(toCompute.substring(0, toCompute.lastIndexOf("."))+"Report.pba");
      }

      if(!job.skipExecution)
      {
        if(job.executor!=null)
        {
          Log.info("Executing Job : "+job.name);
          try
          {
            if(!job.executor.ExecuteTest(job))
              job.skipPlot = true;
          }
          catch(Exception ex)
          {
            Log.error("Could not exeucute job : "+job.name, ex);
            job.skipPlot = true;
          }
          catch(Throwable t)
          {
            Log.error("Could not exeucute job : "+job.name, t);
            job.skipPlot = true;
          }
        }
        else      
          Log.error("No executor for job "+job.name);
      }
      System.gc();
      job.state = JobState.Executed;
    }
  }

  public class CompareThread extends Thread
  {
    public TestJob job;
    public void run()
    {
      if(job.PlottableResults && !job.skipPlot)
      {          
        for(int i=0; i<job.baselineFiles.size(); i++)
        {
          CSVComparison compare = new CSVComparison();
          compare.limit = job.percentDifference;
          compare.reportDifferences = false;          
          compare.setFullReportPath(job.reportFiles.get(i)); 
          if(new File(job.computedFiles.get(i)).exists())
          {
            Set<String> failures = null;
            if(new File(job.baselineFiles.get(i)).exists())
            {
              failures = compare.compare(job.baselineFiles.get(i), job.computedFiles.get(i));
              if(failures==null)// Something bad happened in running this test...
                compare.createErrorSuite(job.name,"Could not compare these files for some reason: "+job.baselineFiles.get(i)+" and " + job.computedFiles.get(i));
            }
            else
            {
              compare.createErrorSuite(job.name,"Basline file not found : "+job.baselineFiles.get(i)+" and " + job.computedFiles.get(i));
            }
            
            compare.write();
           
            if((job.plotType == PlotType.FastPlotErrors || job.plotType == PlotType.FullPlotErrors) && (failures==null || failures.isEmpty()))
            {
              Log.info("No plots for "+job.computedFiles.get(i));
            }
            else
            {
              CSVComparePlotter plotter = new CSVComparePlotter();
              plotter.plotType = job.plotType;
              plotter.plot(job.baselineFiles.get(i), job.computedFiles.get(i), failures);
              /*
              PlotDriver driver = new PlotDriver();
              if(job.plotType == PlotType.FastPlot)
              {
                driver.preload = true;
                driver.onlyPlotFailures = false;
                driver.abbreviateContents = 5;
              }
              else if(job.plotType == PlotType.FullPlot)
              {
                driver.preload = true;
                driver.onlyPlotFailures = false;
                driver.abbreviateContents = 0;
              }
              else if(job.plotType == PlotType.FastPlotErrors)
              {
                driver.preload = true;
                driver.onlyPlotFailures = true;
                driver.abbreviateContents = 5;
              }
              else if(job.plotType == PlotType.FullPlotErrors)
              {
                driver.preload = true;
                driver.onlyPlotFailures = true;
                driver.abbreviateContents = 0;
              }
              else if(job.plotType == PlotType.MemoryFastPlot)
              {
                driver.preload = false;
                driver.onlyPlotFailures = false;
                driver.abbreviateContents = 0;
              }
              if(driver.preload)
              {
                if(!driver.preloadData(job.baselineFiles.get(i), job.computedFiles.get(i)))
                  continue;
              }
              if(job.executor.getClass().getName().indexOf("Scenario")!=-1)
                driver.isScenario = true;

              driver.generateCompareJobs(job.baselineFiles.get(i), job.computedFiles.get(i), failures);
              driver.execute();
               */
            }
          }
          else
          {
            Log.error("Couldn't read file "+job.computedFiles.get(i));
          }
        }
      }
      System.gc();
      job.state = JobState.Complete;
    }
  }

  public void createReport()
  {
    List<String> currentGroup;
    SETestReport report = new SETestReport();    
    report.setFullReportPath("./test_results/"+this.testName+"Report.pba");    
    for(TestJob job : this.jobs)
    {
      String group = job2groups.get(job);
      if(!groups.containsKey(group))
        groups.put(group,new ArrayList<String>());
      currentGroup = groups.get(group);

      if (job.knownFailure)
      {
        report.addKnownSuiteFailure(job.name);
        continue;
      }    
      if(job.reportFiles.isEmpty())
      {
        report.createErrorSuite(job.name,"No reports found for "+job.name+" to summarize");
        continue;
      }
      for(String reportFile : job.reportFiles)
      {
        if(!FileUtils.fileExists(reportFile))
        {
          report.createErrorSuite(job.name,"Unable to find file "+reportFile+" to summarize");
          continue;
        }
        SETestReport tRpt = new SETestReport();
        try
        {
        	tRpt.readFile(reportFile);
        	for(SETestSuite ts : tRpt.testSuites)
          {
          	report.addTestSuite(ts);
          	currentGroup.add(ts.getName());
          }
        }
        catch(Exception ex)
        {
        	report.createErrorSuite(job.name,reportFile);
        	Log.error("Need file with TestReportData object");
        }
      }
    }    
    report.write();
    // Write the HTML to a file
    try
    {
      BufferedWriter out = new BufferedWriter(new FileWriter("./test_results/"+this.testName+".html"));
      out.write(report.toHTML(this.reportName,groups));
      out.close();
    }
    catch(Exception ex)
    {
      Log.error("Unable to write HTML report for "+this.testName,ex);
    }
  }

  static void DeleteTestResults(String hint, boolean executeJobs, boolean plotResults)
  {// Try and delete all files related to this test
    // There are lots of various naming conventions, and this should support all of them

    // Get rid of any extensions
    hint = hint.replaceAll(".pba", "");
    hint = hint.replaceAll(".txt", "");

    File file;

    if(executeJobs)
    {
      file = new File(hint+".log");
      FileUtils.delete(file);

      file = new File(hint+"Test.log");
      FileUtils.delete(file);

      file = new File(hint+".txt");
      FileUtils.delete(file);

      file = new File(hint+".pba");
      FileUtils.delete(file);

      file = new File(hint+"Test.pba");
      FileUtils.delete(file);

      file = new File(hint+"Report.pba");
      FileUtils.delete(file);

      file = new File(hint+"Results.txt");
      FileUtils.delete(file);
    }

    if(plotResults)
    {
      file = new File(hint+"/");
      FileUtils.delete(file);

      file = new File(hint+"Results");
      FileUtils.delete(file);
    }
  }
}


