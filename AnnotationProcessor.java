package com.wrtsz.intercom.master.annotation.compile;

import com.google.auto.service.AutoService;
import com.wrtsz.intercom.master.annotation.AppProject;
import com.wrtsz.intercom.master.annotation.ReplaceActivity;

import java.io.IOException;
import java.io.Writer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.annotation.processing.AbstractProcessor;
import javax.annotation.processing.Filer;
import javax.annotation.processing.Processor;
import javax.annotation.processing.RoundEnvironment;
import javax.lang.model.SourceVersion;
import javax.lang.model.element.Element;
import javax.lang.model.element.TypeElement;
import javax.lang.model.element.VariableElement;
import javax.lang.model.type.TypeMirror;
import javax.tools.JavaFileObject;

/**
 * Created by huangxueshi on 2018/7/18.
 * 注解处理器，编译时生成java类
 */
@AutoService(Processor.class)
public class AnnotationProcessor extends AbstractProcessor {
    @Override
    public Set<String> getSupportedAnnotationTypes() {
        Set<String> types = new LinkedHashSet<>();
        types.add(AppProject.class.getCanonicalName());
        types.add(ReplaceActivity.class.getCanonicalName());
        return types;
    }

    @Override
    public SourceVersion getSupportedSourceVersion() {
        return SourceVersion.latestSupported();
    }


    @Override
    public boolean process(Set<? extends TypeElement> set, RoundEnvironment roundEnvironment) {
        System.out.println("-----------------process-----------------------");



        Map<String, List<TypeElement>> cacheMap = new HashMap<>();

        //遍历所有AppProject注解
        Set<? extends Element> elementSet = roundEnvironment.getElementsAnnotatedWith(ReplaceActivity.class);
        for (Element element : elementSet) {

            TypeElement variableElement = (TypeElement) element;
            String  activityName = getActivityName(variableElement);
            List<TypeElement> list = cacheMap.get(activityName);
            if (list == null) {
                list=new ArrayList<>();
                cacheMap.put(activityName, list);
            }

            list.add(variableElement);
            System.out.println("--------->"+variableElement.getSimpleName().toString());
        }

        String pkg = "com.wrtsz.intercom.master.view";
        String clsName = "AppActivityWrapper";
        List<String> lines = new LinkedList<>();
        List<String> mapLines = new LinkedList<>();
        lines.add("package com.wrtsz.intercom.master.view;");
        lines.add("import android.text.TextUtils;");
        lines.add("import com.wrtsz.commonplatform.prop.PropertiesLoader;");
        lines.add("import java.util.HashMap;");
        lines.add("import java.util.Map;");
        lines.add("import java.util.List;");
        lines.add("import java.util.LinkedList;");
        lines.add("import com.wrtsz.intercom.master.model.bean.AppActivity;");


        try {
            Filer filer = processingEnv.getFiler();
            JavaFileObject javaFileObject = filer.createSourceFile(pkg + "." + clsName);
            Writer writer = javaFileObject.openWriter();

            Set<String> activityNameSets = cacheMap.keySet();
            if (activityNameSets != null) {
                for (String activityName : activityNameSets) {
                    List<TypeElement> caheElements = cacheMap.get(activityName);
                    if (caheElements.size() == 1) {
                        TypeElement typeElement = caheElements.get(0);
                        AppProject appProject = typeElement.getAnnotation(AppProject.class);
                        ReplaceActivity replaceActivity = typeElement.getAnnotation(ReplaceActivity.class);

                        if (replaceActivity != null) {
                            lines.add("import " +  getActivityName(typeElement) + ";");
                            lines.add("import "  + replaceActivity.value()+ ";");

                            String key = replaceActivity.value().substring(
                                    replaceActivity.value().lastIndexOf(".") + 1);

                            mapLines.add("\tappActivityMap = new HashMap<>();");
                            String content = "";
                            if (appProject != null) {
                                String values = "new String[]{";
                                for (int i = 0; i < appProject.value().length; i++) {
                                    values += "\"" + appProject.value()[i] + "\"";
                                    if (i < appProject.value().length - 1) {
                                        values += ",";
                                    } else {
                                        values += "}";
                                    }
                                }
                                content = "         appActivityMap.put(" + key + ".class," + "\n"
                                        + "new AppActivity(" +values + "," +  "\"" + getActivityName(typeElement)   + "\"));";

                            } else {
                                content = "         appActivityMap.put(" + key + ".class," + "\n"
                                        + "new AppActivity(null," + "\"" + getActivityName(typeElement)   + "\"));";
                            }

                            mapLines.add(content);
                            mapLines.add("\tappActivityMaps.add(appActivityMap);\n");
                        }

                    }
                }
            }

            lines.add("/**\n\n" +
                    " * AUTO GENERATE,DO NOT MODIFY\n" +
                    " * AUTHOR:huangxueshi\n\n" +
                    " */");
            lines.add("public class " + clsName + "  {");
            lines.add("   private static final AppActivityWrapper ourInstance = new AppActivityWrapper();");
            lines.add("   public static AppActivityWrapper getInstance() {\n" +
                    "           return ourInstance;\n" +
                    "       }");

            lines.add("   private List<Map<Class,AppActivity>> appActivityMaps = new LinkedList<>();");
            lines.add("   Map<Class,AppActivity> appActivityMap = new HashMap<>();");

            //添加构造方法
            lines.add("   private AppActivityWrapper() {");
            for (String mapLine : mapLines) {
                lines.add(mapLine);
            }
            lines.add("   }");

            //添加getter
            lines.add("  public AppActivity get(Class actviityCls) {\n" +
                    "       for (Map<Class,AppActivity> appActivityMap : appActivityMaps) {\n" +
                    "           AppActivity appActivity =  appActivityMap.get(actviityCls);\n" +
                    "           if (appActivity != null) {\n" +
                    "               String appProject = PropertiesLoader.getProjectPropFileName();\n" +
                    "               String[] values = appActivity.getAppProjects();\n" +
                    "               if (values != null) {\n" +
                    "                   for (String value : values) {\n" +
                    "                       if (appProject.equals(value)) {\n" +
                    "                           return appActivity;\n" +
                    "                       }\n" +
                    "                   }\n" +
                    "               } else {\n" +
                    "                   return appActivity;\n" +
                    "               }\n" +
                    "\n" +
                    "           }\n" +
                    "       }\n" +
                    "\n" +
                    "        return null;\n" +
                    "  }");
            lines.add("}");

            writeLines(writer,lines);

            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }



        return false;
    }

    private void writeLines(Writer writer, List<String> headers ) {

        try {
           for (String header : headers) {
               writer.write(header);
               writer.write("\n");
           }
        } catch (IOException e) {
            e.printStackTrace();
        }


    }

    private String getActivityName(TypeElement variableElement) {
        String packageName = getPackageName(variableElement);
        return packageName+"."+ variableElement.getSimpleName().toString();
    }

    private String getPackageName(TypeElement variableElement) {
        String packageName = processingEnv.getElementUtils().getPackageOf(variableElement).getQualifiedName().toString();
        System.out.println("-------packageName--------"+packageName);
        return packageName;
    }
}
