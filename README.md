# QRedmineApi
A small library for working with the Redmine Rest Api

Небольшая библиотека для работы с Redmine Rest Api.

Основные описания функций находятся прямо в коде. Можно содать Doxygen, но описал я не всё.

Пример кода:

{

    FileToken token;
        if(!filePath.isNull()) {
            token = rest->uploadFile(filePath);
        }
        
      Issue iss;
      
      Item         priority = {2, "Нормальный"};       ///< Priority
      Item         project = {240, "Планы отделов"};        ///< Project
      Item         status = {44, "На согласовании"};         ///< Status
      Item         tracker = {47, "Индивидуальный план"};        ///< Tracker

      iss.project = project;
      iss.tracker = tracker;
      iss.status = status;
      iss.priority = priority;

      iss.author.id = curr_vers->author_id.toUInt();
      iss.author.name = curr_vers->author->fio;

      iss.assignedTo.id = assignet_to.id.toInt();
      iss.assignedTo.name = assignet_to.fio;

      QString subject = QString("Задача");;

      iss.subject = subject;

      iss.version.id = curr_vers->id.toInt();
      iss.version.name = curr_vers->name;

      if(!description.isNull())
          iss.description = description;

      iss.startDate = current_date;

      iss.dueDate = Calendar::getLastWorkDay(current_date, 3);

      if(!token.isNull()) {
          Upload upload;
          QFileInfo info(filePath);
          upload.token = token.token;
          upload.filename = info.fileName();
          upload.content_type = "application/pdf";
          iss.uploads.append(upload);
      }

      rest->sendIssue(iss, curr_vers->author->login);
}
    
